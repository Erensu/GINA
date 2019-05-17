
$PSCommandPath
$PS_Main_path = $PSCommandPath | Split-Path -Parent


$EMS2IONEX = join-path $PS_Main_path -ChildPath "EMS2IONEX.exe"


# Get EMS file names and make hash table basod on day
$EMS = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath  "ems"
$EMSFiles = Get-ChildItem $EMS  -Filter *.ems

$EMSTable =  @{}

#Foreach ($file in $EMSFiles) {
 $EMSFiles | %{   

    $file = $_
    $day = [int]$file.Name.SubString(7,3)

    $EMSTable.add($day, $file)

}

# If EMS and IONEX is available for the same day run IONEXComparision.exe
foreach($day in $(218..365)){

    if ($EMSTable[$day]){

        $year = [int]$EMSTable[$day].BaseName.SubString(3,2)
        $OutPutName = join-path $EMS -ChildPath $(-join($EMSTable[$day].BaseName, ".", $year, "I"))
        $OutPutName


        & "$EMS2IONEX" @($EMSTable[$day].FullName, $OutPutName, 0, 3600, 1)

    }
}

