function posarray = readposfile(posfile)
  #open file
  [fide, errormsg] = fopen(posfile);
  if errormsg
    error('%s file open error\n', posfile);
    exit
  end

  #get number of header lines
  head_lines = 0;
  while ~feof(fide)
      tline = fgetl(fide);
      answer = findstr(tline,'%');
      head_lines++;
      if isempty(answer)
        break;
      end  
  end
  head_lines--;
  printf("file: %s header lines: %d\n", posfile, head_lines);

  #read header
  frewind(fide);
  for i = 1:head_lines, line = fgetl(fide); end;

  #read data into A matrix
  posarray = fscanf(fide,"%d/%d/%d %d:%d:%f %f %f %f %d %d %f %f %f %f %f %f %f %f", [19 Inf]);
  fclose(fide);
  printf("file: %s data lines: %d\n", posfile, size(posarray, 2));
endfunction
