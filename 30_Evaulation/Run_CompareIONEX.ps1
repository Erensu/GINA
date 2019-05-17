
$PSCommandPath
$PS_Main_path = $PSCommandPath | Split-Path -Parent


$CompareIONEX = join-path $PS_Main_path -ChildPath "CompareIONEX.exe"

$Comp_Path = $(join-path $PS_Main_path -ChildPath "2018"| join-path -ChildPath  "comp")

if(!$(Test-Path $Comp_Path)){

    New-Item -Path $(join-path $PS_Main_path -ChildPath "2018"| join-path -ChildPath  "comp") -ItemType Directory

}


# Get IONEX file names and make hash table basod on day
$IONEX = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath  "code"
$IONEXFiles = Get-ChildItem $IONEX  -Filter *.18I
$IONEXTable =  @{}

 $IONEXFiles | %{   

    $file = $_
    $day = [int]$file.Name.SubString(4,3)

    $IONEXTable.add($day, $file)

}

# Get EMS file names and make hash table basod on day
$EMS = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath  "ems"
$EMSFiles = Get-ChildItem $EMS  -Filter *.18I

$EMSTable =  @{}

#Foreach ($file in $EMSFiles) {
 $EMSFiles | %{   

    $file = $_
    $day = [int]$file.Name.SubString(7,3)

    $EMSTable.add($day, $file)

}

# If EMS and IONEX is available for the same day run IONEXComparision.exe
foreach($day in $(1..365)){

    if ($EMSTable[$day] -and $IONEXTable[$day]){

        $OutPutName = join-path $Comp_Path -ChildPath $(-join("Comp_", $IONEXTable[$day].BaseName, "-", $EMSTable[$day].BaseName, $IONEXTable[$day].Extension))
        $OutPutName

        #$Parms = $(-join( $IONEXTable[$day], " ", $EMSTable[$day], " ", $OutPutName, " ", 600))
        #$Parms
        #$Prms = $Parms.Split(" ")
        #$Prms

        & "$CompareIONEX" @($IONEXTable[$day].FullName, $EMSTable[$day].FullName, $OutPutName, 600)

    }
}

