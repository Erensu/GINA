%octave script to read ionex file and draw Total Electron Content map(s)

clear all; close all; clc; %page_screen_output(0)
folder = fileparts(which(mfilename)); 
addpath(genpath(folder));

%ionex file name as the first argument
%if nargin == 1
%  arg_list = argv ();
%  ionex_file = arg_list{1};
%else
  ionex_file = '\PositionComparisonInputFiles\bute0190\TestIonexStoreOut.19i';  %just for testing
  plusFilePath2Pics = 'IonexDifference\NotInterpolated';
%end
filewPath = which(ionex_file);

%File = GetFullPath(ionex_file, 'lean');
[filepath,name,ext] = fileparts(filewPath);

if exist(filewPath, 'file') == 2
    
else
    error('%s file open error\n', filewPath);
end


[fin, errormsg] = fopen(filewPath, 'r');
if errormsg
  error('%s file open error\n', filewPath);
  exit
end

%map of the world
world_map = load('coastline.txt');

n_maps=0; n_rms=0;
while ~feof(fin)
    tline = fgetl(fin);
    if strfind(tline, 'LAT1 / LAT2 / DLAT')
      lat1 = str2num(tline(2:8));
      lat2 = str2num(tline(9:15));
      dlat = str2num(tline(16:21));
      lat = lat1:dlat:lat2;
    end
    if strfind(tline, 'LON1 / LON2 / DLON')
      lon1 = str2num(tline(2:8));
      lon2 = str2num(tline(9:15));
      dlon = str2num(tline(16:21));
      lon = lon1:dlon:lon2;
      o = size(lon, 2);  %number of values in a block
      n = fix(o / 16) + 1;            %number of rows in a block
    end
    if (~isempty(strfind(tline, 'START OF TEC MAP')) || ~isempty(strfind(tline, 'START OF RMS MAP')))
%todo: check lat, lon vectors exist
      type_of_data = tline(70:73);
      if strfind(type_of_data,'TEC')
        n_maps=n_maps+1;                           %index of current map
      else
        n_rms=n_rms+1;
      end  
      tline = fgetl(fin);
      if strfind(tline, 'EPOCH OF CURRENT MAP')
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
              tec_data = str2double(tline((k-1)*5+1:k*5)) * 0.1;  %dimension of data is 0.1 TECU
              if tec_data < 300
                  ii = ii + 1;
                  tec_map(ii, 3) = tec_data;
                  tec_map(ii, 2) = lat(i);
                  tec_map(ii, 1) = lon(l);
              end
            end
          end
        end
        if(isempty(tec_map))
            continue;
        end
  %short output of current map
        if strfind(type_of_data,'TEC')
          fprintf ('%d TEC MAP read %d %02d %02d %02d:%02d:%02d\n', n_maps, year, month, day, hour, minute, sec);
        else
          fprintf ('%d RMS MAP read %d %02d %02d %02d:%02d:%02d\n', n_rms, year, month, day, hour, minute, sec);
        end  
        fprintf ('number of data %d\n', size(tec_map, 1));
        fprintf ('max %.1f min %.1f in TECU\n', max(tec_map(:,3)), min(tec_map(:,3)));
  %plot TEC MAP
        map = figure('visible','off');
        scatter(tec_map(:,1), tec_map(:,2), 50, tec_map(:,3), 's', 'filled');
        hold on;
        %drawing coastlines
        plot(world_map(:,1),world_map(:,2),'k');
        %country borders
        %https://www.mathworks.com/matlabcentral/fileexchange/50390-borders
        %borders('countries','k')
        colormap(jet);
        title(colorbar,'TEC [TECU]')
        ylabel('Latitude [deg]')
        xlabel('Longitude [deg]')
        axis([-60 60 10 90]);
        caxis([0 100]);
        if strfind(type_of_data,'TEC')
          title (sprintf('Total Electron Content Map %d %02d %02d %02d:%02d:%02d', year, month, day, hour, minute, sec));
          print(map, fullfile(filepath,plusFilePath2Pics,sprintf(strcat(strrep(strcat(name,ext),'.','_'), '_iono%02d'), n_maps)),'-dpng');
        else
          title (sprintf('Total Electron Content RMS Map %d %02d %02d %02d:%02d:%02d', year, month, day, hour, minute, sec));
          print(map, fullfile(filepath,plusFilePath2Pics,sprintf(strcat(strrep(strcat(name,ext),'.','_'), '_rms%02d'), n_rms)),'-dpng');
        end
        close(map);
      end
    end
end
fclose(fin);
