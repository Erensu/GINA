%reads rtklib output position file and get statistics
%Bence Takács 09-02-2018

#input file
if nargin == 1
  arg_list = argv ();
  posfile = arg_list{1};
else  
  posfile='pos/bute_007_2018_sbas.pos'; #just for testing
end  

#reference coordinates, BUTE
fi0=47.480948420157;
la0=19.0565382131225;
h0=180.797;

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
A = fscanf(fide,"%d/%d/%d %d:%d:%f %f %f %f %d %d %f %f %f %f %f %f %f %f", [19 Inf]);
fclose(fide);
printf("file: %s data lines: %d\n", posfile, size(A, 2));

#get true coordinate errors
A(7,:) = 31 * 3600 * (A(7,:)-fi0);
A(8,:) = 31 * 3600 * cosd(fi0) * (A(8,:)-la0);
A(9,:) = A(9,:)-h0;

#output the statistics
printf("mean errors: %.3f %.3f %.3f\n", mean(A(7,:)), mean(A(8,:)), mean(A(9,:)));
printf("standard deviatons: %.3f %.3f %.3f\n", std(A(7,:)), std(A(8,:)), std(A(9,:)));
printf("max errors:  %.3f %.3f %.3f\n", max(abs(A(7,:))), max(abs(A(8,:))), max(abs(A(9,:))));
