/interface bridge add name=bridge10;
/interface bridge add name=bridge11;
/interface bridge port remove [find interface=ether10];
/interface bridge port remove [find interface=ether11];
/interface bridge port remove [find interface=ether12];
/interface bridge port add bridge=bridge10 interface=ether10;
/interface bridge port add bridge=bridge10 interface=ether11;
/interface bridge port add bridge=bridge11 interface=ether12;
/interface bridge port remove [find interface=ether13];
/interface bridge port add bridge=bridge11 interface=ether13;
/interface bridge port remove [find interface=ether14];
/interface bridge port add bridge=bridge11 interface=ether13;
