# smallrx

This is an amateur radio receiver in no more than 100 lines of code.  
(Much of the lines are actually comments.)

It can receive AM/NFM/SSB transmissions with an RTL-SDR.

:capital_abcd: The block diagram of its internals:

```                       
   IQ samples from SDR HW -> [U8->float conversion] -> [Frequency translation] ----------+
                                                                                         |
   audio output <- [float->S16 conversion] <+- [FM demod] <-+ [Decimating FIR filter]  <-+
                                            +- [AM demod] <-+
                                            +- [SSB demod] <+
```

:hammer: How to build:
```bash
git clone https://github.com/ha7ilm/smallrx
cd smallrx
make
```

:hash: Command line parameters:
```bash
./rx <center> <rx_freq> <l[sb]|u[sb]|a[m]|f[m]>
```

:headphones: To demodulate NFM at 145.325 MHz using an RTL-SDR:
```bash
rtl_sdr -f145300000 - | ./rx 145300000 145325000 f | aplay -c1 -fS16_LE -r48000
```

:headphones: To demodulate AM at 0.873 MHz using a HackRF:
```bash
hackrf_transfer -s 2400000 -f 893000 -a1 -r- | csdr convert_s8_f | csdr fir_decimate_cc 10 | csdr gain_ff 30 | csdr convert_f_u8 | ./rx 893000 873000 a | mplayer -cache 1024 -quiet -rawaudio samplesize=2:channels=1:rate=48000 -demuxer rawaudio -
```

To test throughput:
```bash
cat /dev/urandom | ./rx 145300000 145325000 f | csdr through > /dev/null
```


