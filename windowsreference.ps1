$documents = [environment]::getfolderpath("mydocuments")
$path = "$documents\PairStorm"
If(!(Test-Path $path))
{
      New-Item -ItemType Directory -Force -Path $path
}
if (!(Test-Path "$path\reference"))
{
	$url = "http://upload.cppreference.com/mwiki/images/b/b2/html_book_20190607.zip"
	$output = "$path\cppref.zip"

	$wc = New-Object System.Net.WebClient
	$wc.DownloadFile($url, $output)
#OR
	(New-Object System.Net.WebClient).DownloadFile($url, $output)
	Expand-Archive -Path "$path\cppref.zip" -DestinationPath "$path"
	Remove-Item "$path\cppref.zip"
	Move-Item -Path "$path\cppref\reference" -Destination $path
	Remove-Item -LiteralPath "$path\cppref" -Force -Recurse
}