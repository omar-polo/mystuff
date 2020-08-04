/* $OpenBSD$ */
/*
 * revert of https://github.com/godotengine/godot/pull/2840
 *
 * original code by Anton Yabchinskiy aka a12n on github,
 * adapted to compile on newer versions of godot.
 */

#include "audio_driver_ao.h"

#ifdef AO_ENABLED

#include "core/os/os.h"
#include "core/project_settings.h"

#include <stdio.h>

#include <cstring>

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

Error AudioDriverAO::init() {
	active = false;
	thread_exited = false;
	exit_thread = false;
	pcm_open = false;
	samples_in = NULL;

	mix_rate = 44100;
	speaker_mode = SPEAKER_MODE_STEREO;
	channels = 2;

	ao_sample_format format;

	format.bits = 16;
	format.rate = mix_rate;
	format.channels = channels;
	format.byte_format = AO_FMT_LITTLE;
	format.matrix = (char*)"L,R";

	device = ao_open_live(ao_default_driver_id(), &format, NULL);
	ERR_FAIL_COND_V(device == NULL, ERR_CANT_OPEN);

	int latency = GLOBAL_DEF("audio/output_latency", 25);
	buffer_size = nearest_power_of_2(latency * mix_rate / 1000);

	samples_in = memnew_arr(int32_t, buffer_size * channels);

	mutex = Mutex::create();
	thread = Thread::create(AudioDriverAO::thread_func, this);

	return OK;
}

void AudioDriverAO::thread_func(void *p_udata) {
	AudioDriverAO *ad = (AudioDriverAO*)p_udata;

	// overwrite samples on conversion
	int16_t *samples_out = reinterpret_cast<int16_t*>(ad->samples_in);
	unsigned int n_samples = ad->buffer_size * ad->channels;

	// why sizeof? isn't it 2 by definition?
	unsigned int n_bytes = n_samples * sizeof(int16_t);

	while (!ad->exit_thread) {
		if (ad->active) {
			ad->lock();
			ad->audio_server_process(ad->buffer_size, ad->samples_in);
			ad->unlock();

			for (unsigned int i = 0; i < n_samples; i++) {
				samples_out[i] = ad->samples_in[i] >> 16;
			}
		} else {
			memset(samples_out, 0, n_bytes);
		}

		if (ad->exit_thread)
			break;

		if (!ao_play(ad->device, reinterpret_cast<char*>(samples_out), n_bytes)) {
			ERR_PRINT("ao_play() failed");
		}
	}

	ad->thread_exited = true;
}

void AudioDriverAO::start() {
	active = true;
}

int AudioDriverAO::get_mix_rate() const {
	return mix_rate;
}

AudioDriver::SpeakerMode AudioDriverAO::get_speaker_mode() const {
	return speaker_mode;
}

void AudioDriverAO::lock() {
	if (!thread || !mutex)
		return;
	mutex->lock();
}

void AudioDriverAO::unlock() {
	if (!thread || !mutex)
		return;
	mutex->unlock();
}

void AudioDriverAO::finish() {
	if (!thread)
		return;

	exit_thread = true;
	Thread::wait_to_finish(thread);

	if (samples_in)
		memdelete_arr(samples_in);

	memdelete(thread);
	if (mutex)
		memdelete(mutex);
	if (device)
		ao_close(device);

	thread = NULL;
}

AudioDriverAO::AudioDriverAO() {
	mutex = NULL;
	thread = NULL;

	ao_initialize();
}

AudioDriverAO::~AudioDriverAO() {
	ao_shutdown();
}

#endif
