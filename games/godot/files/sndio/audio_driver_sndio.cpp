/* $OpenBSD$ */
/*
 * (setq tab-width 8)
 * (setq indent-tabs-mode t)
 * (setq c-basic-offset 8)
 */

#include "audio_driver_sndio.h"

#ifdef SNDIO_ENABLED

#include "core/os/os.h"
#include "core/project_settings.h"

#include <stdio.h>

Error AudioDriverSndio::init() {
	active = false;
	thread_exited = false;
	exit_thread = false;
	speaker_mode = SPEAKER_MODE_STEREO;

	/* XXX: SIO_PLAY|SIO_REC */
	handle = sio_open(SIO_DEVANY, SIO_PLAY, 0);
	ERR_FAIL_COND_V(handle == NULL, ERR_CANT_OPEN);

	struct sio_par par;
	sio_initpar(&par);

	par.bits = 16;
	par.rate = GLOBAL_GET("audio/mix_rate");
	// par.appbufsz = closest_power_of_2(latency * par.rate / 1000) * channels;
	// printf("requested appbufsz: %u\n", par.appbufsz);

	/*
	 * XXX: require SIO_SYNC instead of SIO_IGNORE (the default) ?
	 * what is the most sensible choice for a game?
	 */
	// par.xrun = SIO_ERROR;

	if (!sio_setpar(handle, &par))
		/* XXX: close the handle here? */
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	if (!sio_getpar(handle, &par))
		/* XXX: close the handle here? */
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	mix_rate = par.rate;
	channels = par.pchan;
	buffer_size = par.appbufsz; // / channels;

	samples_in.resize(buffer_size * channels);
	samples_out.resize(buffer_size * channels);

	int global_mix_rate = GLOBAL_GET("audio/mix_rate");
	int global_output_latency = GLOBAL_GET("audio/output_latency");

	printf("computed latency: %u\n", par.bufsz * 1000 / mix_rate);

	printf("global mix rate:       %d\n", global_mix_rate);
	printf("global output latency: %d\n", global_output_latency);
	printf("mix rate:              %d\n", mix_rate);
	printf("buffer size:           %zu\n", buffer_size);
	printf("channels:              %d\n", channels);
	printf("bufsz:                 %u\n", par.bufsz);
	printf("appbufsz:              %u\n", par.appbufsz);
	printf("round:                 %u\n", par.round);

	if (!sio_start(handle)) {
		ERR_PRINTS("sio_start failed");
		exit_thread = true;
	}

	mutex = Mutex::create();
	thread = Thread::create(AudioDriverSndio::thread_func, this);

	return OK;
}

void AudioDriverSndio::thread_func(void *p_udata) {
	AudioDriverSndio *ad = (AudioDriverSndio*)p_udata;

	while (!ad->exit_thread) {
		ad->lock();

		if (!ad->active) {
			ad->unlock();
			OS::get_singleton()->delay_usec(1000);
			ad->lock();

			for (size_t i = 0; i < ad->buffer_size; ++i)
				ad->samples_out.write[i] = 0;
		 } else {
			ad->audio_server_process(ad->buffer_size, ad->samples_in.ptrw());

			for (size_t i = 0; i < ad->buffer_size; ++i)
				ad->samples_out.write[i] = ad->samples_in[i] >> 16;
		}

		size_t todo = ad->buffer_size * ad->channels * sizeof(int16_t);
                size_t total = 0;

		while (todo != 0  /* && !ad->exit_thread */) {
			const uint8_t *src = (const uint8_t*)ad->samples_out.ptr();
			// printf("old %zu\n", total * ad->channels);
			// printf("new %zu\n", total);

			// size_t wrote = sio_write(ad->handle, (void*)(src + total * ad->channels), todo);
			size_t wrote = sio_write(ad->handle, (void*)(src + total), todo);

			if (wrote == 0) {
				if (sio_eof(ad->handle)) {
					ERR_PRINTS("sndio: fatal error");
					ad->active = false;
					ad->exit_thread = true;
					break;
				} else {
					ERR_PRINTS("sndio: temp error?");

                                        ad->unlock();

					OS::get_singleton()->delay_usec(1000);

					ad->lock();
				}
			} else {
				total += wrote;
				todo  -= wrote;
			}
		}

		ad->unlock();
	}

	ad->thread_exited = true;
}

void AudioDriverSndio::start() {
	active = true;
}

int AudioDriverSndio::get_mix_rate() const {
	return mix_rate;
}

AudioDriver::SpeakerMode AudioDriverSndio::get_speaker_mode() const {
	return speaker_mode;
}

void AudioDriverSndio::lock() {
	if (!thread || !mutex)
		return;
	mutex->lock();
}

void AudioDriverSndio::unlock() {
	if (!thread || !mutex)
		return;
	mutex->unlock();
}

void AudioDriverSndio::finish() {
	if (handle)
		sio_close(handle);
	handle = NULL;

	if (!thread)
		return;

	exit_thread = true;
	Thread::wait_to_finish(thread);

	memdelete(thread);
	thread = NULL;

	if (mutex) {
		memdelete(mutex);
		mutex = NULL;
	}
}

AudioDriverSndio::AudioDriverSndio() {
	mutex = NULL;
	thread = NULL;
}

AudioDriverSndio::~AudioDriverSndio() {
}

#endif
