#include "bmcl/ringbucket.h"

#include "Arduino.h"

ringbucket_t bucket;
uint8_t bdata[100];

void setup() {
    ringbucket_init(&bucket, bdata, 100);
}

void loop() {
    uint8_t test[11];
    ringbucket_append(&bucket, test, 11);
    ringbucket_remove_first(&bucket);
}

