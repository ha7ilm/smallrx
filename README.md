# smallrx

This is an amateur radio receiver in 64 lines.

It does the following:

```                       
   IQ samples from SDR HW -> [U8->float conversion] -> [Frequency translation] ----------+
                                                                                         |
   audio output <- [float->S16 conversion] <+- [FM demod] <-+ [Decimating FIR filter]  <-+
                                            +- [AM demod] <-+
                                            +- [SSB demod] <+
```

To run (RTL-SDR, 145.325 MHz NFM):
```bash
rtl_sdr -f145300000 - | ./rx 145300000 145325000 f | aplay -c1 -fS16_LE -r48000
```

To test throughput:
```bash
cat /dev/urandom | ./rx 145300000 145325000 f | csdr through > /dev/null
```

To run (HackRF, 0.873 MHz AM):
```bash
hackrf_transfer -s 2400000 -f 893000 -a1 -r- | csdr convert_s8_f | csdr fir_decimate_cc 10 | csdr gain_ff 30 | csdr convert_f_u8 | ./rx 893000 873000 a | mplayer -cache 1024 -quiet -rawaudio samplesize=2:channels=1:rate=48000 -demuxer rawaudio -
```



