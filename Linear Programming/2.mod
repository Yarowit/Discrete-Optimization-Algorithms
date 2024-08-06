# zad 1
# Jarosław Socha

set F; 
/* canning plants */

set L;
/* markets */

set K dimen 2;
/* transportation lane */


param zapas{f in F};
/* capacity of plant i in cases */

param zapotrzebowanie{l in L};
/* demand at market j in cases */

param koszt{f in F, l in L};
/* distance in thousands of miles */

# param f;
# /* freight in dollars per case per thousand miles */

table Firmy IN "CSV" "Firma.csv" :
  F <- [Firma], zapas ~ zapasy;

table Lotniska IN "CSV" "Lotnisko.csv" :
  L <- [Lotnisko], zapotrzebowanie ~ zapotrzebowanie;

table Koszty IN "CSV" "Koszt.csv" :
  K <- [Firma, Lotnisko], koszt ~ koszt;

var x{(i,j) in K} >= 0;
#galony przesłane
/* shipment quantities in cases */

minimize cost: sum{(i,j) in K} koszt[i,j] * x[i,j];
/* total transportation costs in thousands of dollars */

subject to supply{f in F}: sum{(f,l) in K} x[f,l] <= zapas[f];

subject to demand{l in L}: sum{(f,l) in K} x[f,l] = zapotrzebowanie[l];

# s.t. supply{i in I}: sum{(i,j) in K} x[i,j] <= a[i];
# /* observe supply limit at plant i */

# s.t. demand{j in J}: sum{(i,j) in K} x[i,j] >= b[j];
# /* satisfy demand at market j */

solve;

printf "Koszt: %d \n", cost;
# for{f in F}{
#     printf "%d: %d", f, zapas[f]- (sum{l in L} x[f,l]);
# }
# printf ""
# table tab_result{(i,j) in K} OUT "CSV" "result.csv" :
#   i ~ plant, j ~ market, x[i,j] ~ shipment;

table tab_result{(i,j) in K} OUT "CSV" "result.csv" :
  i ~ firma, j ~ lotnisko, x[i,j] ~ dostawa;

end;
