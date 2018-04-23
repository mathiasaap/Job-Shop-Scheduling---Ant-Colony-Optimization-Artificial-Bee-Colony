#ifndef graphNode_hpp
#define graphNode_hpp

class graphNode
{
public:
    const int startTime, endTime, machineNo, jobNo;
    graphNode(int startTime, int endTime,  int machineNo, int jobNo): startTime(startTime), endTime(endTime), machineNo(machineNo), jobNo(jobNo){}
};
#endif /* graphNode_hpp */
