
% K�pes beolvasni egy trajectoria f�jlt. Ezt ki kell tudni plotolni.
% LLH egy plot de k�l�n subploton a koordin�t�k. ECEF-fel ugyanez. 
% F�ggelten att�l, hogy a traject�ria f�jlban, hogy volt megadva, az ECEF �s
% LLH kel. 
% Sz�val tudni kell �tv�ltani is.
% 
% K�l�n ploton �br�zolni a be�ll�si sz�get. roll pitch yaw, 3 subploton
% 2 D-s ploton �br�zolnal a lat longot. Matlabos 2D-s plot.
% 2D es ploton �br�zoljuk a NED-et. Ehhez az els� kezd�pontot v�lasszuk
% �referinci�nak 
% �s mindig ahhoz k�pest sz�moljuk ki a Cne-t �s az elmozdul�sokat.
% 
% Ugyanez a plot csak 3D-ben.
% 
% 
% Google mapet csin�lni az llh-b�l. (err�l van matlabos script a neten, csak 
% t�rni kell)
% 
% 3D ploton �br�zolni a trajeckt�ri�t.
% 
% Olvassuk be az IMU f�jlt.
% Ha van a traject�ria f�jlhoz megfelel� IMU f�jl, akkor a 3D-s ploton
% �br�zolj�k
% �az er�ket is az ismert pontokban.
% 
% 
% Plusz feladat:
% A 3D plotot az er�kkel anim�ljuk. Id�ben l�tsz�djon, ahogy haladunk, �s
% ahogy m�s, m�s er�k hatnak r�.


function = data = read_data
data_switch = 0; 
key_word = 'START OF DATA';
ginaFile = fopen('imu.gina');
row = 1;
while ~feof(ginaFile)
    %disp(line);
    line = fgetl(ginaFile);
    if data_switch == 1
       data_cells = strsplit(line);
       for col = 1:length(data_cells)
            data(row,col) = str2double(cell2mat(data_cells(col)));
       end
       row=row+1;
    end
    if strcmp(line,key_word) == 1
        data_switch = 1;
    end
end
disp(data)
fclose(ginaFile);




















