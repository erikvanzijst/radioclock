#ifndef FIRMWARE_DCF_H
#define FIRMWARE_DCF_H

int32_t dcf_init(void (* sync_cb)(struct calendar_date_time *cal_dt));
int32_t dcf_deinit(void);
void dcf_data_isr(void);

#endif //FIRMWARE_DCF_H
