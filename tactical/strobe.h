#ifndef HEXBRIGHT_STROBE
#define HEXBRIGHT_STROBE

class strobe {
  private:
    static void _pulse(unsigned long lvl, unsigned long interval);
    static void _set_strobe(unsigned int interval);
  public:
    static void pulse(unsigned long lvl, unsigned long interval) { _pulse(lvl, interval); }
    static void set_strobe(unsigned int interval) { _set_strobe(interval); }
}
#endif
