function [tec_map, year, month, day, hour, minute, sec] = readBlock( fin, lat, lon, unitOfData)


%todo: check lat, lon vectors exist

      tline = fgetl(fin);
      if strfind(tline, 'EPOCH OF CURRENT MAP')
          
        o = size(lon, 2);               %number of values in a block
        n = fix(o / 16) + 1;            %number of rows in a block
      
        year = str2num(tline(1:6));
        month = str2num(tline(7:12));
        day = str2num(tline(13:18));
        hour = str2num(tline(19:24));
        minute = str2num(tline(25:30));
        sec = str2num(tline(31:36));
        tec_map = [];                 %init block
        ii = 0;
        for i=1:size(lat, 2)
          tline = fgetl(fin);
          if ~ strfind(tline, 'LAT/LON1/LON2/DLON/H')
            error('file error at line %s\n', tline);
            exit
          end
          l = 0;
          for j=1:n
            tline = fgetl(fin);
            if j < n
               m = 16;  %number of data in n-1 th row
            else
               m = o - (j-1)*16;    %number of data in the last row
            end
            for k=1:m
              l=l+1;
              %tec_map(i,l) = str2double(tline((k-1)*5+1:k*5)) * 0.1;  %dimension of data is 0.1 TECU
              tec_data = str2double(tline((k-1)*5+1:k*5));  %dimension of data is 0.1 TECU
              
              if tec_data < 9999
                tec_data = tec_data * unitOfData;
              
                  ii = ii + 1;
                  tec_map(ii, 3) = tec_data;
                  tec_map(ii, 2) = lon(l) ;
                  tec_map(ii, 1) = lat(i);
              end
            end
          end
        end
        if(isempty(tec_map))
            return;
        end
  
  
       
      end

    
end