#ifndef FIRMWARE_DCF_H
#define FIRMWARE_DCF_H

enum dcf_error_t {
    SUCCESSFUL,
    TIMEOUT
};

enum dcf_error_t dcf_sync(int32_t max_millis);

#endif //FIRMWARE_DCF_H
