#!/bin/bash

touch AlarmPJ.service

echo "[Unit]" > AlarmPJ.service
echo "Description=AlarmPJ service" >> AlarmPJ.service
echo "[Service]" >> AlarmPJ.service
echo "User=$USERNAME" >> AlarmPJ.service
echo "Group=$USERNAME" >> AlarmPJ.service
echo "WorkingDirectory=$PWD/bin" >> AlarmPJ.service
echo "ExecStart=/bin/bash -ce \"./alarmd 2> ../src/alarmd.errors.log\"" >> AlarmPJ.service
echo "[Install]" >> AlarmPJ.service
echo "WantedBy=multi-user.target" >> AlarmPJ.service


sudo mv AlarmPJ.service -t /etc/systemd/system/

systemctl start AlarmPJ.service
systemctl status AlarmPJ.service