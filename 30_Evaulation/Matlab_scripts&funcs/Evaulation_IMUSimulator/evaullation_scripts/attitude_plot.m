% K�l�n ploton �br�zolni a be�ll�si sz�get. roll pitch yaw, 3 subploton OK
function attitude_plot(t_data) 
% % legyen save strukt�ra,
% �s egy kapcsaol� jhogy mentesen vagy ne, Kommentezve nem j� ! 
    figure
    title('Roll,pitch �s yaw sz�gek')
    subplot(3,1,1);
    plot(t_data.roll);
    
    xlabel('Id�l�p�s');% id�l�p�s-> id� [s] vagy valami 
    ylabel('Roll [rad]');
    title('Roll');
    grid on;

    subplot(3,1,2);
    plot(t_data.pitch);
    xlabel('Id�l�p�s');
    ylabel('Pitch [rad]');
    title('Pitch');
    grid on;
    
    
    subplot(3,1,3);
    plot(t_data.yaw);
    xlabel('Id�l�p�s');
    ylabel('Yaw [rad]');
    title('Yaw');
    grid on;
% varargin varargout = functuon overload 

%     Save to file
%     print(filenamewithfullpath,'-dpng')
%     h=gcf;
%     set(h,'PaperOrientation','landscape');
%     set(h,'PaperUnits','normalized');
%     set(h,'PaperPosition', [0 0 1 1]);
%     print(filenamewithfullpath,'-dpdf')
%     savefig(h,filenamewithfullpath)
%     close all
end 