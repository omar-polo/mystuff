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
	pcm_open = false;
	samples_in = NULL;

	speaker_mode = SPEAKER_MODE_STEREO;

	// mix_rate = 44100;
	// channels = 2;

	struct sio_par par;

	/* XXX: SIO_PLAY|SIO_REC ? */
	handle = sio_open(SIO_DEVANY, SIO_PLAY, 0);
	ERR_FAIL_COND_V(handle == NULL, ERR_CANT_OPEN);

	sio_initpar(&par);
	/* TODO: customize par? */
	sio_getpar(handle, &par);

	mix_rate = par.rate;
	channels = par.pchan;

	printf("bufsize:  %u\n", par.bufsz);
	printf("appbufsz: %u\n", par.appbufsz);

	GLOBAL_DEF("audio/output_latency", par.bufsz);

	buffer_size = par.appbufsz;
	samples_in = memnew_arr(int32_t, buffer_size /* * channels */);

	printf("mix rate is %u\n", mix_rate);
	printf("channels is %d\n", channels);

	mutex = Mutex::create();
	thread = Thread::create(AudioDriverSndio::thread_func, this);

	return OK;
}

void AudioDriverSndio::thread_func(void *p_udata) {
	AudioDriverSndio *ad = (AudioDriverSndio*)p_udata;

	int16_t *samples_out = reinterpret_cast<int16_t*>(ad->samples_in);
        size_t nsamples = ad->buffer_size * ad->channels;
        size_t nbytes = nsamples * sizeof(int16_t);

        printf("buffer_size: %zu\nnsamples = %zu\nnbytes = %zu\n", ad->buffer_size, nsamples, nbytes);

	while (!ad->exit_thread) {
		if (ad->active) {
			ad->lock();
			ad->audio_server_process(ad->buffer_size, ad->samples_in);
			ad->unlock();

			for (unsigned int i = 0; i < nsamples; ++i)
				samples_out[i] = ad->samples_in[i] >> 16;
		} else
			memset(samples_out, 0, nbytes);

		if (ad->exit_thread)
			break;

		size_t wrote = 0;
		do {
			wrote += sio_write(ad->handle, reinterpret_cast<void*>(&samples_out[wrote]),
			    nbytes - wrote);
		} while (wrote != nbytes);
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
	if (!thread)
		return;

	exit_thread = true;
	Thread::wait_to_finish(thread);

	if (samples_in)
		memdelete_arr(samples_in);

	memdelete(thread);
	if (mutex)
		memdelete(mutex);
	if (handle)
		sio_close(handle);

	thread = NULL;
}

AudioDriverSndio::AudioDriverSndio() {
	mutex = NULL;
	thread = NULL;
}

AudioDriverSndio::~AudioDriverSndio() {
}

#endif
