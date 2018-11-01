#!/usr/bin/env bash

sonar-scanner \
-Dsonar.projectKey=tesModulesFramework \
-Dsonar.organization=elominp-github \
-Dsonar.sources=./src \
-Dsonar.host.url=https://sonarcloud.io  \
-Dsonar.login=2ae8b7eb5b9261d855f203212dadb827248f059b \
-Dsonar.cfamily.build-wrapper-output.bypass=true
