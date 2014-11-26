/*
 * cRecorder.h
 *
 *  Created on: 26 lis 2014
 */

#ifndef CRECORDER_H_
#define CRECORDER_H_

#include "libs.h"
#include "cSound.h"

class cAlarm {

};

class cAlarmSoundRecorder: public sf::SoundBufferRecorder {
	virtual bool OnStart() {
		std::cout << "Start sound recorder" << std::endl;
		return true;
	}

	/**
	 * Audio samples are provided to the onProcessSamples function every 100 ms.
	 * This is currently hard-coded into SFML and you can't change that
	 * (unless you modify SFML itself).
	 */
	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		unsigned int SampleRate = GetSampleRate();

		auto sound = std::make_shared<cSound>(false);
		sf::SoundBuffer buff = GetBuffer();
		buff.LoadFromSamples(Samples, SamplesCount, 2, SampleRate);

		auto wasAlarm = sound->ProccessRecording(Samples, SamplesCount, SampleRate);

		if (wasAlarm) saveBuffToFile(Samples, SamplesCount, SampleRate, sound->currentDateTime());


		// return true to continue the capture, or false to stop it
		return true;
	}

	void saveBuffToFile(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate, std::string filename) {
		filename += ".wav";
		sf::SoundBuffer buff = GetBuffer();

		_info("samples count: " << buff.GetSamplesCount() << ", duration: " << buff.GetDuration());
		if(!buff.SaveToFile(filename)) _erro(filename << " not saved :( ");
	}



	virtual void OnStop() {
		std::cout << "Stop sound recorder" << std::endl;
	}
};

class cRecorder {
public:
	cRecorder();
	virtual ~cRecorder();

	const bool fromMicrophoneMode;
	void startRecording();

private:
	cAlarmSoundRecorder Recorder;

	void waitForExitKey();
};

#endif /* CRECORDER_H_ */