


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
%% Az adatok ut�n nem �llhat �res sor! 
%%�s az adatok sorok k�z�tt sem lehet �res sor!
function data = read_data(fileName)
format long;
ginaFile = fopen(fileName);
key_word = 'START OF DATA'; % �rz�keny a spcaekre !!!! a fileban 
data_switch = 0; 
row = 1;
while ~feof(ginaFile)
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
fclose(ginaFile);
end


















