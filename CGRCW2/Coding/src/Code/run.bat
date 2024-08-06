@echo off
set /p file_location="Enter the file location: "
set /p file_name="Enter the output file name: "
raytracer %file_location% > output/%file_name%.ppm