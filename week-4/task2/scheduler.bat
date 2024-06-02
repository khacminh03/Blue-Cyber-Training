@echo off
SCHTASKS /create /tn MyScheduledTask /tr "E:\blue-cyber-re\week-4\task1\modifying.exe" /sc once /st 08:34 /sd 28/05/2024 /ru System
