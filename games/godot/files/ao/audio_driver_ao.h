/* $OpenBSD$ */
/*
 * revert of https://github.com/godotengine/godot/pull/2840
 *
 * original code by Anton Yabchinskiy aka a12n on github,
 * adapted to compile on newer versions of godot.
 */

#include "servers/audio_server.h"

#ifdef AO_ENABLED

#include "core/os/mutex.h"
#include "core/os/thread.h"

#include <ao/ao.h>

class AudioDriverAO : public AudioDriver {
	Thread *thread;
	Mutex *mutex;

	ao_device *device;
	int32_t *samples_in;

	static void thread_func(void*);
	int buffer_size;

	unsigned int mix_rate;
	int channels;
	bool active;
	bool thread_exited;
	mutable bool exit_thread;
	bool pcm_open;
	SpeakerMode speaker_mode;

public:
	const char *get_name() const {
		return "libao";
	}

	virtual Error init();
	virtual void start();
	virtual int get_mix_rate() const;
	virtual SpeakerMode get_speaker_mode() const;
	virtual void lock();
	virtual void unlock();
	virtual void finish();

	AudioDriverAO();
	~AudioDriverAO();
};

#endif
