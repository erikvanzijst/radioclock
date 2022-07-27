#ifndef FIRMWARE_DCF_H
#define FIRMWARE_DCF_H

void dcf_init(void (* sync_cb)(struct calendar_date_time *cal_dt));
void dcf_data_isr(void);

#endif //FIRMWARE_DCF_H
