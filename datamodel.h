#ifndef DATAMODEL_H
#define DATAMODEL_H

class DataModel
{
private:
    bool gbEnabled;
    bool bfEnabled;
    bool edEnabled;
    DataModel(){}
    DataModel(DataModel const&);
    void operator=(DataModel const&);

public:
    static DataModel& getInstance() {
        static DataModel instance;
        return instance;
    }

    bool getGBEnabled(){return gbEnabled;}
    bool getBFEnabled(){return bfEnabled;}
    bool getEDEnabled(){return edEnabled;}
    void setGBEnabled(bool b){gbEnabled=b;}
    void setBFEnabled(bool b){bfEnabled=b;}
    void setEDEnabled(bool b){edEnabled=b;}
};

#endif // DATAMODEL_H
