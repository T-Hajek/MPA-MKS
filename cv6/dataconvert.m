

t = ntc.t;
r = ntc.r;

ad = (r./(r + 10)) * 2^10;

p = polyfit(ad, t, 10);

ad2 = 0:1023;
t2 = round(polyval(p, ad2), 1);

dlmwrite('data.dlm', t2*10, ',');

plot(ad, t, 'bo');
hold on;
plot(ad2, t2, 'r');


hold on, plot(ad2, t2, 'r');