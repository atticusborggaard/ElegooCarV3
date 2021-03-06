#ifndef __ELEGOO_DRIVER_BASE_H__
#define __ELEGOO_DRIVER_BASE_H__

#include "ElegooConstants.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"

class ElegooDriverBase
{
protected:
	int safetyDistanceInCM;
	ElegooDistanceUnit & distUnit;
	ElegooMotorUnit & motorUnit;

public:
	ElegooDriverBase(int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			safetyDistanceInCM(pSafetyDistanceInCM), distUnit(pDistUnit), motorUnit(pMotorUnit)
	{
	}

	virtual ~ElegooDriverBase()
	{
	}

	virtual int processCommand(ElegooCommand cmd) = 0;

protected:

	int checkFrontForObstacles(ElegooCommand cmd)
	{
		if (cmd == ElegooCommand::NO_COMMAND || cmd == ElegooCommand::MOVE_FORWARDS)
		{
			// check front sensor and stop
			const int frontDistance = distUnit.frontDistance();
			if (frontDistance < safetyDistanceInCM)
			{
				motorUnit.stopMoving();
				return ElegooConstants::STOPPED;
			}
		}

		return ElegooConstants::OK;
	}

	ElegooMotorUnit & backOut()
	{
		bool doBackOut = true;
		int rightDistance = 0;
		int leftDistance = 0;

		do
		{
			if (motorUnit.hasCommand())
			{
				// escape from the loop !!
				return motorUnit;
			}

			motorUnit.moveBackwards().stopMoving();
			rightDistance = distUnit.rightDistance();
			leftDistance = distUnit.leftDistance();

			doBackOut = (rightDistance <= safetyDistanceInCM) && (leftDistance <= safetyDistanceInCM);

		} while (doBackOut);

		// here either (rightDistance > safetyDistanceInCM) || (leftDistance > safetyDistanceInCM)
		if (rightDistance > safetyDistanceInCM)
		{
			return motorUnit.turnRight();
		}
		else
		{
			return motorUnit.turnLeft();
		}
	}
};

#endif
