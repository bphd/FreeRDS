# FreeRDS: FreeRDP Remote Desktop Services (RDS)
#
# Copyright 2013-2014 Marc-Andre Moreau <marcandre.moreau@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if(UNIX AND NOT APPLE)
	set(PIXMAN_FEATURE_TYPE "REQUIRED")
	set(PIXMAN_FEATURE_PURPOSE "Pixel Manipulation")
	set(PIXMAN_FEATURE_DESCRIPTION "Pixel Manipulation Library")
	find_feature(Pixman ${PIXMAN_FEATURE_TYPE} ${PIXMAN_FEATURE_PURPOSE} ${PIXMAN_FEATURE_DESCRIPTION})

	add_subdirectory(X11)
endif()

add_subdirectory(RDP)

if(WITH_NETSURF)
	add_subdirectory(NetSurf)
endif()
