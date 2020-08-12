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

#include <poll.h>
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
	par.appbufsz = 2205; // par.round ;
	// par.appbufsz = closest_power_of_2(latency * par.rate / 1000) * channels;
	printf("requested appbufsz: %u\n", par.appbufsz);

	/*
	 * XXX: require SIO_SYNC instead of SIO_IGNORE (the default) ?
	 * what is the most sensible choice for a game?
	 */
	par.xrun = SIO_ERROR;

	if (!sio_setpar(handle, &par))
		/* XXX: close the handle here? */
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	if (!sio_getpar(handle, &par))
		/* XXX: close the handle here? */
		ERR_FAIL_COND_V(1, ERR_CANT_OPEN);

	mix_rate = par.rate;
	channels = par.pchan;
	period_size = par.appbufsz;

	samples_in.resize(period_size * channels);
	samples_out.resize(period_size * channels);

	int global_mix_rate = GLOBAL_GET("audio/mix_rate");
	int global_output_latency = GLOBAL_GET("audio/output_latency");

	printf("computed latency: %u\n", (par.bufsz/par.bps) * 1000 / mix_rate);
	// GLOBAL_DEF("audio/latency", par.bufsz * 1000 / mix_rate);
	// GLOBAL_DEF("audio/output_latency", 30);

	printf("global mix rate:       %d\n", global_mix_rate);
	printf("global output latency: %d\n", global_output_latency);
	printf("mix rate:	       %d\n", mix_rate);
	printf("buffer size:	       %zu\n", period_size);
	printf("channels:	       %d\n", channels);
	printf("bufsz:		       %u\n", par.bufsz);
	printf("appbufsz:	       %u\n", par.appbufsz);
	printf("round:		       %u\n", par.round);
	printf("par.bps		       %u\n", par.bps);
	printf("par.bits	       %u\n", par.bits);

	// if (!sio_start(handle)) {
	// 	ERR_PRINTS("sio_start failed");
	// 	exit_thread = true;
	// }

	mutex = Mutex::create();
	thread = Thread::create(AudioDriverSndio::thread_func, this);

	return OK;
}

void AudioDriverSndio::thread_func(void *p_udata) {
	AudioDriverSndio *ad = (AudioDriverSndio*)p_udata;
	int was_active = 0;

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

		if (was_active) {
			int nfds = sio_pollfd(ad->handle, pfds, POLLOUT);
			if (nfds > 0) {
				// printf("before poll\n");
				if (poll(pfds, nfds, -1) < 0) {
					ERR_PRINTS("sndio: poll failed");
					ad->active = false;
					ad->exit_thread = true;
					ad->stop_counting_ticks();
					ad->unlock();
					break;
				}
				// printf("after poll\n");
			}
			// revents = sio_revents(hdl, pfds);
		}

		ad->lock();
		ad->start_counting_ticks();

		if (!ad->active) {
			if (was_active) {
				was_active = 0;
				sio_stop(ad->handle);
			}

			ad->stop_counting_ticks();
			ad->unlock();
			continue;

			// for (size_t i = 0; i < ad->period_size * ad->channels; ++i) {
			// 	ad->samples_out.write[i] = 0;
			// }
		} else {
			if (!was_active) {
				was_active = 1;
				sio_start(ad->handle);
			}

			// printf("getting fresh audio data\n");
			ad->audio_server_process(ad->period_size, ad->samples_in.ptrw());

			for (size_t i = 0; i < ad->period_size*ad->channels; ++i) {
				ad->samples_out.write[i] = ad->samples_in[i] >> 16;
			}
		}

		// ad->stop_counting_ticks();
		// ad->unlock();

		// ad->lock();
		// ad->start_counting_ticks();

		size_t left = ad->period_size * ad->channels * sizeof(int16_t);
		size_t wrote = 0;

		while (left != 0 && !ad->exit_thread) {
			// if (true) {
			// 	OS::get_singleton()->delay_usec(1000);
			// 	break;
			// }

			const uint8_t *src = (const uint8_t*)ad->samples_out.ptr();
			size_t w = sio_write(ad->handle, (void*)(src + wrote), left);

			if (w == 0) {
				ERR_PRINTS("sndio: fatal error");
				ad->active = false;
				ad->exit_thread = true;
				break;
			} else {
				wrote += w;
				left  -= w;
			}
		}

		ad->stop_counting_ticks();
		ad->unlock();
	}

	printf("sndio end of loop\n");
	ad->thread_exited = true;
}

void AudioDriverSndio::start() {
	printf("sndio: start\n");
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
	printf("sndio: finish\n");

	if (handle) {
		sio_close(handle);
		handle = NULL;
	}

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
