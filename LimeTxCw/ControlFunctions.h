#pragma once


int handle_syscmd(const std::string s);
// help
int handle_antenna(const std::string s);
int handle_ant_bw(const std::string s);
int handle_temp(const std::string s);
int handle_devid(const std::string s);
int handle_freq( const std::string s);
int handle_gain_db(const std::string s);
int handle_enab_chan(const std::string s);
int handle_samp_rate(const std::string s);
int handle_loadcnf(const std::string s);
int handle_savecnf(const std::string s);
int handle_clock_frq(const std::string s);
int handle_reset();
int handle_close();

