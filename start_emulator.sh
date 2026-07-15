#!/bin/bash
cd ~/aosp_local/aosp
source build/envsetup.sh
lunch sdk_phone_x86_64
export ANDROID_PRODUCT_OUT=~/aosp_local/aosp/out/target/product/emulator_x86_64
emulator
