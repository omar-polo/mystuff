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

unsigned int nearest_power_of_2(unsigned int v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

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

	par.rate = GLOBAL_GET("audio/mix_rate");
        par.appbufsz = 15;

	/*
	 * XXX: require SIO_SYNC instead of SIO_IGNORE (the default) ?
	 * what is the most sensible choice for a game?
	 */

	if (!sio_setpar(handle, &par))
		/* XXX: close the handle here? */
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	if (!sio_getpar(handle, &par))
		/* XXX: close the handle here? */
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	mix_rate = par.rate;
	channels = par.pchan;
	buffer_frames = par.appbufsz;
	buffer_size = par.appbufsz * channels;

	samples_in.resize(buffer_size);
	samples_out.resize(buffer_size);

	int global_mix_rate = GLOBAL_GET("audio/mix_rate");
	int global_output_latency = GLOBAL_GET("audio/output_latency");

	printf("global mix rate:       %d\n", global_mix_rate);
	printf("global output latency: %d\n", global_output_latency);
	printf("mix rate:              %d\n", mix_rate);
	printf("buffer frames:         %zu\n", buffer_frames);
	printf("buffer size:           %zu\n", buffer_size);
	printf("channels:              %d\n", channels);
	printf("bufsz:                 %u\n", par.bufsz);
	printf("appbufsz:              %u\n", par.appbufsz);
	printf("round:                 %u\n", par.round);

	// GLOBAL_DEF("audio/output_latency", par.bufsz * par.bps * channels);
	// GLOBAL_DEF("audio/output_latency", par.bufsz/1000);

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
		ad->start_counting_ticks();

		if (!ad->active) {
			for (size_t i = 0; i < ad->buffer_size; ++i)
				ad->samples_out.write[i] = 0;
		 } else {
			ad->audio_server_process(ad->buffer_frames, ad->samples_in.ptrw());

			for (size_t i = 0; i < ad->buffer_size; ++i)
				ad->samples_out.write[i] = ad->samples_in[i] >> 16;
		}

		size_t todo = ad->buffer_size;
                size_t total = 0;

		while (todo && !ad->exit_thread) {
			const uint8_t *src = (const uint8_t*)ad->samples_out.ptr();
			size_t wrote = sio_write(ad->handle, (void*)(src + total * ad->channels), todo);

			if (wrote == 0) {
				if (sio_eof(ad->handle)) {
					ERR_PRINTS("sndio: fatal error");
					ad->active = false;
					ad->exit_thread = true;
					break;
				} else {
					ERR_PRINTS("sndio: temp error?");

					ad->stop_counting_ticks();
					ad->unlock();

					OS::get_singleton()->delay_usec(1000);

					ad->lock();
					ad->start_counting_ticks();
				}
			} else {
				total += wrote;
				todo  -= wrote;
			}
		}

		ad->stop_counting_ticks();
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
