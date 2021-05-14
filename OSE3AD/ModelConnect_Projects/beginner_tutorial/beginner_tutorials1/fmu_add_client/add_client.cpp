#define MODEL_IDENTIFIER add_client

#define MODEL_GUID "{3437763C-DFC5-4959-927B-A4F91B027E28}"

#include "fmi2Functions.h"

#define NUM_REAL_IN 2
#define NUM_REAL_OUT 1

#include <string.h>
#include <time.h>

#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"


// This is the data structure that represents one FMU. A pointer to
// the struct is returned in instantiate and passed to all subsequent
// calls.
typedef struct {
    // Basic information for the FMU.
    char * instanceName;
    char * fmuGUID;
    char * fmuResourceLocation;

    // Memory Management and Logging functions. No other memory or
    // logging functions must be used.
    fmi2CallbackFunctions * functions;

    fmi2Real rIn[NUM_REAL_IN];
    fmi2Real rOut[NUM_REAL_OUT];

    ros::NodeHandle * n;
    ros::ServiceClient client;

} Component;


#if defined(WIN32)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

std::string server_name("add_two_ints");

// -----------------------------------------------------------------------------
// General FMU Functions
//
// TODO: Implement according to the standard.
//
DLL_EXPORT const char * fmi2GetTypesPlatform(void) {
    return "default";
}

DLL_EXPORT const char * fmi2GetVersion(void) {
    return "2.0";
}

DLL_EXPORT fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t comp, const fmi2String comps[]) {
    return fmi2OK;
}


// -----------------------------------------------------------------------------
// Instantiation and freeing of FMUs
//
// TODO: Implement according to the standard.
//
DLL_EXPORT fmi2Component fmi2Instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID, fmi2String fmuResourceLocation, const fmi2CallbackFunctions* functions, fmi2Boolean visible, fmi2Boolean loggingOn) {
    Component * comp = NULL;
    ros::VP_string vp;
    std::string client_name("add_two_client_fmu_");
    client_name += instanceName;

    if (fmuType != fmi2CoSimulation) {
        return NULL;
    }

    comp = (Component *) functions->allocateMemory(1, sizeof(Component));

    comp->instanceName = (char *) functions->allocateMemory(strlen(instanceName) + 1, sizeof(char));
    strcpy(comp->instanceName, instanceName);

    comp->fmuGUID = (char *) functions->allocateMemory(strlen(fmuGUID) + 1, sizeof(char));
    strcpy(comp->fmuGUID, fmuGUID);

    comp->fmuResourceLocation = (char *) functions->allocateMemory(strlen(fmuResourceLocation) + 1, sizeof(char));
    strcpy(comp->fmuResourceLocation, fmuResourceLocation);

    comp->functions = (fmi2CallbackFunctions *) functions;

    comp->functions->logger(comp->functions->componentEnvironment, comp->instanceName,
        fmi2OK, "logAll", "pre init");

    ros::init(vp, client_name);

    comp->functions->logger(comp->functions->componentEnvironment, comp->instanceName,
        fmi2OK, "logAll", "post init");

    ros::NodeHandle * n = new ros::NodeHandle();

    comp->functions->logger(comp->functions->componentEnvironment, comp->instanceName,
        fmi2OK, "logAll", "post new");

    comp->n = n;


    comp->client = comp->n->serviceClient<beginner_tutorials::AddTwoInts>(server_name);


    return comp;
}

DLL_EXPORT void fmi2FreeInstance(fmi2Component c) {
    Component * comp = (Component *) c;

    comp->functions->freeMemory(comp->instanceName);
    comp->functions->freeMemory(comp->fmuGUID);
    comp->functions->freeMemory(comp->fmuResourceLocation);

    delete comp->n;

    comp->functions->freeMemory(c);
}

// -----------------------------------------------------------------------------
// Starting and stopping of simulations
//
// TODO: Implement according to the standard. Note that the state
// diagram has to be followed.
//
DLL_EXPORT fmi2Status fmi2SetupExperiment(fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real tolerance, fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime) {
    // Setup one simulation run
    Component * comp = (Component *) c;

    comp->rOut[0] = comp->rIn[0] + comp->rIn[1];

    return fmi2OK;
}

// Initialization of output values
DLL_EXPORT fmi2Status fmi2EnterInitializationMode(fmi2Component c) {
    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2ExitInitializationMode(fmi2Component c) {
    Component * comp = (Component *) c;

    comp->rOut[0] = 0.;

    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2Terminate(fmi2Component c) {
    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2Reset(fmi2Component c) {
    return fmi2OK;
}



// -----------------------------------------------------------------------------
// Data transfer from and to the FMU
//
// fmi2GetX provides the FMU-Master with values from the FMU
// fmi2SetX provides the FMU with values from the FMU-Master
//
// Set/Get values according to their type/value reference. Note
// that they have to match the type/value in the modelDescription.xml
//
DLL_EXPORT fmi2Status fmi2GetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[]) {
    Component * comp = (Component *) c;

    size_t i;

    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 0 && vr[i] < NUM_REAL_IN) {
            value[i] = comp->rIn[vr[i]];
        } else if (vr[i] >= NUM_REAL_IN && vr[i] < NUM_REAL_IN + NUM_REAL_OUT) {
            value[i] = comp->rOut[vr[i] - NUM_REAL_IN];
        } else {
            return fmi2Error;
        }
    }

    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[]) {
    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[]) {
    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2GetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2String value[]) {
    return fmi2OK;
}


DLL_EXPORT fmi2Status fmi2SetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[]) {
    Component * comp = (Component *) c;

    size_t i;

    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 0 && vr[i] < NUM_REAL_IN) {
            comp->rIn[vr[i]] = value[i];
        } else {
            return fmi2Error;
        }
    }

    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[]) {
    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Boolean value[]) {
    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2SetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2String value[]) {
    return fmi2OK;
}


// -----------------------------------------------------------------------------
// FMU State
//
// Optionally, the FMU can save and load its state
//
DLL_EXPORT fmi2Status fmi2GetFMUstate(fmi2Component c, fmi2FMUstate* s) {
    return fmi2Error;
}
DLL_EXPORT fmi2Status fmi2SetFMUstate(fmi2Component c, fmi2FMUstate s) {
    return fmi2Error;
}
DLL_EXPORT fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* s) {
    return fmi2Error;
}
DLL_EXPORT fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate s, size_t* n) {
    return fmi2Error;
}
DLL_EXPORT fmi2Status fmi2SerializeFMUstate(fmi2Component c, fmi2FMUstate s, fmi2Byte v[], size_t n) {
    return fmi2Error;
}
DLL_EXPORT fmi2Status fmi2DeSerializeFMUstate(fmi2Component c, const fmi2Byte v[], size_t n, fmi2FMUstate* s) {
    return fmi2Error;
}

// -----------------------------------------------------------------------------
// Optional derivative functions
//
DLL_EXPORT fmi2Status fmi2SetRealInputDerivatives (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer order[], const fmi2Real value[]) {
    return fmi2OK;
}
DLL_EXPORT fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer order[], fmi2Real value[]) {
    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2GetDirectionalDerivative(fmi2Component c, const fmi2ValueReference vUnknown_ref[], size_t nUnknown, const fmi2ValueReference vKnown_ref[], size_t vKnown, const fmi2Real dvKnown[], fmi2Real dvUnkown[]) {
    return fmi2OK;
}


// -----------------------------------------------------------------------------
// Functions for FMI2 for Co-Simulation
//

#define SIMPLE_LOG(instance, status, category, message, ...) \
        instance->functions->logger(instance->functions->componentEnvironment, instance->instanceName, status, \
        category, message, ##__VA_ARGS__);

// -----------------------------------------------------------------------------
// Actual simulation of the FMU
//
DLL_EXPORT fmi2Status fmi2DoStep(fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, fmi2Boolean newStep) {
    Component * comp = (Component *) c;
    beginner_tutorials::AddTwoInts srv;

    srv.request.a = static_cast<int64_t> (comp->rIn[0]);
    srv.request.b = static_cast<int64_t> (comp->rIn[1]);
//    std::string server_name("add_two_ints");
//    ros::ServiceClient client = comp->n->serviceClient<beginner_tutorials::AddTwoInts>(server_name);

//    if (client.call(srv)) {
    if (comp->client.call(srv)) {
        comp->rOut[0] = static_cast<fmi2Real> (srv.response.sum);
    } else {
        comp->functions->logger(comp->functions->componentEnvironment, comp->instanceName,
            fmi2Error, "logError", "server '%s' didn't respond", server_name.c_str());
        return fmi2Error;
    }

    return fmi2OK;
}

DLL_EXPORT fmi2Status fmi2CancelStep(fmi2Component c) {
    return fmi2OK;
}


// -----------------------------------------------------------------------------
// FMU Status functions
//
// TODO: Implement according to standard.
//
DLL_EXPORT fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status* value) {
    return fmi2OK;
}
DLL_EXPORT fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real*   value) {
    return fmi2OK;
}
DLL_EXPORT fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer* value) {
    return fmi2OK;
}
DLL_EXPORT fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean* value) {
    return fmi2OK;
}
DLL_EXPORT fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String* value) {
    return fmi2OK;
}

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif /* __cplusplus */