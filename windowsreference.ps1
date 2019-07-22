$path = "$PSScriptRoot\res"
If(!(test-path $path))
{
      New-Item -ItemType Directory -Force -Path $path
}
if (!(Test-Path "PSScriptRoot\res\cppref.zip"))
{
	$url = "http://upload.cppreference.com/mwiki/images/b/b2/html_book_20190607.zip"
	$output = "$PSScriptRoot\res\cppref.zip"

	$wc = New-Object System.Net.WebClient
	$wc.DownloadFile($url, $output)
#OR
	(New-Object System.Net.WebClient).DownloadFile($url, $output)

	Expand-Archive "$PSScriptRoot\res\cppref.zip"
	Remove-Item "$PSScriptRoot\res\cppref.zip"
}