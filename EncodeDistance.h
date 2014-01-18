#ifndef ENCODEDISTANCE_H
#define ENCODEDISTANCE_H

#include <Encoder.h>

class EncodeDistance
{
public:
    EncodeDistance::EncodeDistance(Encoder*);
    ~EncodeDistance();
    double convertTickToDist(double distance);
    float convertDistToTick(float distance);
    void Start();
    double GetDistance();
private:
    Encoder* myEncoder;
};

#endif // ENCODEDISTANCE_H
