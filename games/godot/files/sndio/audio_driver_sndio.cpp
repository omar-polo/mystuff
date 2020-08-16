/* $OpenBSD$ */

#include "audio_driver_sndio.h"

#ifdef SNDIO_ENABLED

#include "core/os/os.h"
#include "core/project_settings.h"

#include <poll.h>
#include <stdio.h>

Error AudioDriverSndio::init() {
	active = false;
	thread_exited = false;
	exit_thread = false;
	speaker_mode = SPEAKER_MODE_STEREO;
	was_active = 0;

	handle = sio_open(SIO_DEVANY, SIO_PLAY, 0);
	ERR_FAIL_COND_V(handle == NULL, ERR_CANT_OPEN);

	struct sio_par par;
	sio_initpar(&par);

	par.bits = 16;
	par.rate = GLOBAL_GET("audio/mix_rate");

	/*
	 * XXX: require SIO_SYNC instead of SIO_IGNORE (the default) ?
	 * what is the most sensible choice for a game?
	 */
	// par.xrun = SIO_ERROR;

	if (!sio_setpar(handle, &par))
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	if (!sio_getpar(handle, &par))
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	mix_rate = par.rate;
	channels = par.pchan;
	period_size = par.appbufsz;

	samples_in.resize(period_size * channels);
	samples_out.resize(period_size * channels);

	mutex = Mutex::create();
	thread = Thread::create(AudioDriverSndio::thread_func, this);

	return OK;
}

void AudioDriverSndio::thread_func(void *p_udata) {
	AudioDriverSndio *ad = (AudioDriverSndio*)p_udata;

	int nfds = sio_nfds(ad->handle);
	struct pollfd *pfds;
	if ((pfds = (struct pollfd*)calloc(sizeof(*pfds), nfds)) == NULL) {
		ERR_PRINTS("cannot allocate memory");
		ad->active = false;
		ad->exit_thread = true;
		ad->thread_exited = true;
		return;
	}

	while (!ad->exit_thread) {
		if (ad->was_active) {
			nfds = sio_pollfd(ad->handle, pfds, POLLOUT);
			if (nfds > 0) {
				if (poll(pfds, nfds, -1) < 0) {
					ERR_PRINTS("sndio: poll failed");
					ad->exit_thread = true;
					break;
				}
			}
		}

		ad->lock();
		ad->start_counting_ticks();

		if (!ad->active) {
			if (ad->was_active) {
				ad->was_active = 0;
				sio_stop(ad->handle);
			}

			ad->stop_counting_ticks();
			ad->unlock();
			/* XXX: sleep a bit here? */
			continue;
		} else {
			if (!ad->was_active) {
				ad->was_active = 1;
				sio_start(ad->handle);
			}

			ad->audio_server_process(ad->period_size, ad->samples_in.ptrw());

			for (size_t i = 0; i < ad->period_size*ad->channels; ++i) {
				ad->samples_out.write[i] = ad->samples_in[i] >> 16;
			}
		}

		size_t left = ad->period_size * ad->channels * sizeof(int16_t);
		size_t wrote = 0;

		while (left != 0 && !ad->exit_thread) {
			const uint8_t *src = (const uint8_t*)ad->samples_out.ptr();
			size_t w = sio_write(ad->handle, (void*)(src + wrote), left);

			if (w == 0 && sio_eof(ad->handle)) {
				ERR_PRINTS("sndio: fatal error");
				ad->exit_thread = true;
			} else {
				wrote += w;
				left  -= w;
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
	if (thread) {
		exit_thread = true;
		Thread::wait_to_finish(thread);

		memdelete(thread);
		thread = NULL;
	}

	if (mutex) {
		memdelete(mutex);
		mutex = NULL;
	}

	if (handle) {
		sio_close(handle);
		handle = NULL;
	}
}

AudioDriverSndio::AudioDriverSndio() {
	mutex = NULL;
	thread = NULL;
}

AudioDriverSndio::~AudioDriverSndio() {
}

#endif
