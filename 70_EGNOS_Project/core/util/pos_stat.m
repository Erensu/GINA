%reads rtklib output position file and get statistics
%Bence Tak�cs 09-02-2018

close all;

#input file
%if nargin == 1
%  arg_list = argv ();
%  posfile = arg_list{1};
%else  
  posfile1='pos/bute_007_2018_sbas.pos'; #just for testing
  posfile2='pos/bute_007_2018_abs.pos'; #just for testing
%end  

A = readposfile(posfile1);
B = readposfile(posfile2);

#reference coordinates, BUTE
fi0=47.480948420157;
la0=19.0565382131225;
h0=180.797;

#get true coordinate errors
A(7,:) = 31 * 3600 * (A(7,:)-fi0);
A(8,:) = 31 * 3600 * cosd(fi0) * (A(8,:)-la0);
A(9,:) = A(9,:)-h0;
B(7,:) = 31 * 3600 * (B(7,:)-fi0);
B(8,:) = 31 * 3600 * cosd(fi0) * (B(8,:)-la0);
B(9,:) = B(9,:)-h0;

#get time in hour
tA = A(4,:)+A(5,:)/60+A(8,:)/3600;
tB = B(4,:)+B(5,:)/60+B(8,:)/3600;

#output the statistics
printf("mean errors: %.3f %.3f %.3f\n", mean(A(7,:)), mean(A(8,:)), mean(A(9,:)));
printf("standard deviatons: %.3f %.3f %.3f\n", std(A(7,:)), std(A(8,:)), std(A(9,:)));
printf("max errors:  %.3f %.3f %.3f\n", max(abs(A(7,:))), max(abs(A(8,:))), max(abs(A(9,:))));

#plot errors
plot1 = figure();
subplot(4,1,1);
plot(tA, A(7,:),'c');
hold on;
plot(tB, B(7,:),'b');
grid on
ylabel('latitude [m]')
title('True position errors')
axis([0 24 -12 12])
set(gca,'YTick',-12:4:12);
set(gca,'XTick',0:2:24);
annotation('textbox',[.05 0.94 .1 .2],'String',strrep(posfile1,'_','-'),'EdgeColor','none', 'Color','cyan')
annotation('textbox',[.05 0.92 .1 .2],'String',strrep(posfile2,'_','-'),'EdgeColor','none', 'Color','blue')
annotation('textbox',[.7 0.94 .1 .2],'String',sprintf("mean %.2f rms %.2f max %.2f", mean(A(7,:)), std(A(7,:)), max(abs(A(7,:)))),'EdgeColor','none', 'Color','cyan')
annotation('textbox',[.7 0.92 .1 .2],'String',sprintf("mean %.2f rms %.2f max %.2f", mean(B(7,:)), std(B(7,:)), max(abs(B(7,:)))),'EdgeColor','none', 'Color','blue')

subplot(4,1,2);
plot(tA, A(8,:),'c');
hold on;
plot(tB, B(8,:),'b');
grid on
ylabel('longitude [m]')
axis([0 24 -12 12])
set(gca,'YTick',-12:4:12);
set(gca,'XTick',0:2:24);
annotation('textbox',[.7 0.71 .1 .2],'String',sprintf("mean %.2f rms %.2f max %.2f", mean(A(8,:)), std(A(8,:)), max(abs(A(8,:)))),'EdgeColor','none', 'Color','cyan')
annotation('textbox',[.7 0.69 .1 .2],'String',sprintf("mean %.2f rms %.2f max %.2f", mean(B(8,:)), std(B(8,:)), max(abs(B(8,:)))),'EdgeColor','none', 'Color','blue')

subplot(4,1,3);
plot(tA, A(9,:),'c');
hold on;
plot(tB, B(9,:),'b');
grid on
ylabel('height [m]')
axis([0 24 -12 12])
set(gca,'YTick',-12:4:12);
set(gca,'XTick',0:2:24);
annotation('textbox',[.7 0.49 .1 .2],'String',sprintf("mean %.2f rms %.2f max %.2f", mean(A(9,:)), std(A(9,:)), max(abs(A(9,:)))),'EdgeColor','none', 'Color','cyan')
annotation('textbox',[.7 0.47 .1 .2],'String',sprintf("mean %.2f rms %.2f max %.2f", mean(B(9,:)), std(B(9,:)), max(abs(B(9,:)))),'EdgeColor','none', 'Color','blue')

subplot(4,1,4);
plot(tA, A(11,:),'c');
hold on;
plot(tB, B(11,:),'b');

grid on
ylabel('x of sats')
axis([0 24 0 12])
set(gca,'YTick',0:2:12);
set(gca,'XTick',0:2:24);
xlabel('GPS time [hour]')
print(plot1, 'error1.png', '-dpng');

%close(plot1);
