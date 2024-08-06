var F1L1 >= 0; var F1L2 >= 0; var F1L3 >= 0; var F1L4 >= 0;
var F2L1 >= 0; var F2L2 >= 0; var F2L3 >= 0; var F2L4 >= 0;
var F3L1 >= 0; var F3L2 >= 0; var F3L3 >= 0; var F3L4 >= 0;

minimize cost:      10*F1L1 + 7*F2L1  + 8*F3L1 +
                    10*F1L2 + 11*F2L2 + 14*F3L2 +
                    9*F1L3  + 12*F2L3 + 4*F3L3 +
                    11*F1L4 + 13*F2L4 + 9*F3L4 ;

subject to f1:   F1L1 + F1L2 + F1L3 + F1L4  <=  275000;
subject to f2:   F2L1 + F2L2 + F2L3 + F2L4  <=  550000;
subject to f3:   F3L1 + F3L2 + F3L3 + F3L4  <=  660000;

subject to l1:   F1L1 + F2L1 + F3L1 =  110000;
subject to l2:   F1L2 + F2L2 + F3L2 =  220000;
subject to l3:   F1L3 + F2L3 + F3L3 =  330000;
subject to l4:   F1L4 + F2L4 + F3L4 =  440000;

end;