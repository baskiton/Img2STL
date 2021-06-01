#ifndef IMG2STL_WORKERMESSAGE_H_
#define IMG2STL_WORKERMESSAGE_H_


struct WorkerMessage {
    enum wMsgCmd {
        wMSG_STOP = 0,
        wMSG_END,
        wMSG_RUN
    };
    wMsgCmd cmd;
    int x,
        y;
    double z0,
           z1,
           z2;
};

#endif // IMG2STL_WORKERMESSAGE_H_
