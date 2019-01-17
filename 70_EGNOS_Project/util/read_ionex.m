#octave script to read ionex file and draw Total Electron Content map(s)

clear all; close all; clc; page_screen_output(0)

#ionex file name
fin = fopen('corg0140.19i');

#map of the world
world_map = load('coastline.txt');

n_maps=0;
while ~feof(fin)
    tline = fgetl(fin);
    if strfind(tline, 'LAT1 / LAT2 / DLAT')
      lat1 = str2num(tline(2:8));
      lat2 = str2num(tline(9:15));
      dlat = str2num(tline(16:21));
      lat = lat1:dlat:lat2;
    endif
    if strfind(tline, 'LON1 / LON2 / DLON')
      lon1 = str2num(tline(2:8));
      lon2 = str2num(tline(9:15));
      dlon = str2num(tline(16:21));
      lon = lon1:dlon:lon2;
      o = abs((lon1 - lon2)) / dlon;  #number of values in a block
      n = fix(o / 16) + 1;            #number of rows in a block
    endif
    if strfind(tline, 'START OF TEC MAP')
#todo: check lat, lon vectors exist  
      n_maps++;                           #index of current map
      tline = fgetl(fin);
      if strfind(tline, 'EPOCH OF CURRENT MAP')
        year = str2num(tline(1:6));
        month = str2num(tline(7:12));
        day = str2num(tline(13:18));
        hour = str2num(tline(19:24));
        min = str2num(tline(25:30));
        sec = str2num(tline(31:36));
        tec_map = [];                 #init block
        for i=1:size(lat, 2)
          tline = fgetl(fin);
          if ~ strfind(tline, 'LAT/LON1/LON2/DLON/H') 
            break;
          endif
          l = 0;
          for j=1:n
            tline = fgetl(fin);
            m = length(tline) / 5;     #number of data in a row
            for k=1:m
              l++;
              tec_map(i,l) = str2num(tline((k-1)*5+1:k*5)) * 0.1;  #todo: dimension of data, now 0.1 TECU
            endfor
          endfor    
        endfor
  #plot TEC MAP
        map = figure();
        imagesc([-180 180],[90 -90], tec_map);
        hold on;
        plot(world_map(:,1),world_map(:,2),'w');
        colormap(jet); 
        c=colorbar; 
        title(c,'TEC [TECU]')
        axis xy;
        set(gca,'XTick',-180:45:180);
        set(gca,'YTick',-90:30:90);
        caxis([0 60]);
        title (sprintf('Total Electron Content Map %d %02d %02d %02d:%02d%02d', year, month, day, hour, min, sec));
        print(map, sprintf('iono%02d.jpg', n_maps), "-S840,480");
        close(map);
      endif
    endif  
end
fclose(fin);
printf ("%d TEC MAP read\n", n_maps);
