#pragma once

#include "FastTrackIpc/IProcessor.h"

class Tracker : public IProcessor {
	public: // IProtocol implementation

    virtual bool process(IProtocol & protocol) const;

	private: // implementation

		void processIsPrime(IProtocol & protocol) const;

		bool isPrime(int n) const;
};
