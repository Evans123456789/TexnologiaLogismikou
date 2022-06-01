class endocrinologyBloodTestRequest:public baseTestRequest {
public:
    endocrinologyBloodTestRequest(){testType="blood";testName="endocrinologyBloodTest";}
    baseTest* requestTest(){ return new endocrinologyBloodTest;}
};
class cardiologyBloodTestRequest:public baseTestRequest {
public:
    cardiologyBloodTestRequest(){testType="blood";testName="cardiologyBloodTest";}
    baseTest* requestTest(){ return new cardiologyBloodTest;}
};
class doctor {
protected:
    drugInfo* prescribedDrug;
    virtual bool doctorIsAllowed(baseTest* test)=0;
public:
    virtual void prescribe(baseTest* test)
    {
        if(doctorIsAllowed(test)){
            prescribedDrug=test->prescribeRelatedDrug();
            cout<<"Doctor decided to prescribe "<<prescribedDrug->getDrugName()<<endl;
        }
    }
    drugInfo *getPrescribedDrug() { return prescribedDrug; }
};
class endocrinologist:public doctor {
    bool doctorIsAllowed(baseTest* test){ return test->getTestName()=="endocrinologyBloodTest";}
};
class orthopedist:public doctor {
    bool doctorIsAllowed(baseTest* test){ return test->getTestName()=="X-RAY";}
};
class cardiologist:public doctor {
    bool doctorIsAllowed(baseTest* test){ return test->getTestName()=="cardiologyBloodTest"||test->getTestName()=="EKG";}
};
class baseClinic {
protected:
    string clinicName;
    doctor* assignedDoctor;
    vector<baseTestRequest*> requiredTests;
public:
    baseClinic(){}
    ~baseClinic(){ delete assignedDoctor;}
    void assignDoctor(doctor* doctor){assignedDoctor=doctor;}
    string getClinicName() { return clinicName; }
    virtual vector<baseTestRequest*>* getRequiredTests(){ return &requiredTests;}
    doctor* getAssignedDoctor() {return assignedDoctor;}
};
class cardiologyClinic:public baseClinic {
public:
    cardiologyClinic(){
        clinicName="cardiology";
        requiredTests.push_back(new EKGrequest);
        requiredTests.push_back(new cardiologyBloodTestRequest);
    }
};
class orthopedicsClinic:public baseClinic {
public:
    orthopedicsClinic(){
        clinicName="orthopedics";
        requiredTests.push_back(new XRAYrequest);
    }
};
class endocrinologyClinic:public baseClinic {
public:
    endocrinologyClinic(){
        clinicName="endocrinology";
        requiredTests.push_back(new endocrinologyBloodTestRequest);
    }
};
class baseTestDepartment {
protected:
    string expectedTestType;
    baseTest* testResult;
public:
    virtual void createTest(baseTestRequest* request){
        if(request->getType()==expectedTestType){
            testResult=request->requestTest();
            cout<<"A new "<<request->getTestName()<<" test has been done"<<endl;
        }
    }
    string &getExpectedTestType(){return expectedTestType;}
    baseTest* getTestResult(){return testResult;}
};

class radiologyDepartment : public baseTestDepartment {
private:
    radiologyDepartment(){ expectedTestType="radiological";}
    radiologyDepartment(const radiologyDepartment&);
    radiologyDepartment& operator=(const radiologyDepartment&);
    static radiologyDepartment *instance;
    static Mutex mutex;
public:
    static radiologyDepartment *GetInstance(){
        if(instance == NULL){
            mutex.lock();
            if(instance == NULL){
                instance = new radiologyDepartment();
            }
            mutex.unlock();
        }
        return instance;
    }
};
Mutex radiologyDepartment::mutex;
radiologyDepartment* radiologyDepartment::instance = NULL;
class labDepartment:public baseTestDepartment {
public:
    labDepartment(){expectedTestType="blood";}
};
class secretaryCommand {
protected:
    baseClinic* clinic;
    static radiologyDepartment* radiology;
    static vector<labDepartment*>* labDepartments;
    patient* requestingPatient;
public:
    secretaryCommand(){}
    secretaryCommand(baseClinic* givenClinic, patient* givenPatient) {
        clinic=givenClinic;
        requestingPatient=givenPatient;
    }
    ~secretaryCommand() { }
    virtual void execute()=0;
    virtual string getCommandName()=0;
    static void initializeTestDepartments(radiologyDepartment* rad, vector<labDepartment*>* labs) {
        radiology=rad;
        labDepartments=labs;
    }
};
