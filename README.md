# Informations
Un simple xray concu pour Paladium<br>
Nécessite d'être injecté avec l'option "Manual Map" afin de contourner l'anticheat actuel de Paladium.<br>
Une autre possibilité est de renommer le DLL en vlc.dll, mais cela peut être patch assez facilement.

# Comment build le projet
1. Installez un JDK pour Java8
2. Téléchargez Visual Studio, un IDE qui permet de coder en C++ et de facilement build des projets
3. Clonez le projet et ouvrez le .sln fourni dedans avec Visual Studio
4. Importez les libraries et include nécessaire (includes jni/jvmti, libraries: vcruntime140.lib, opengl32.lib)
5. Ctrl+B, puis allez dans le dossier de votre projet, Debug, puis x64, et le DLL build sera là.

# Informations supplémentaire
La library "vcruntime140.lib" est nécessaire afin de ne pas être chargé en tant que module, et par conséquence être détecté par l'anticheat actuel de Paladium.<br>
Vous pouvez évidemment "faire" celle-ci par vous-même, en utilisant un utilitaire "dll2lib" qui "convertira" vcruntime140.dll en vcruntime140.lib

# Screenshot
![screenshot](https://github.com/Wykt/paladium-xray/assets/47749428/b7b0cd25-4904-4242-bd38-b1ea69ef04df)

# License
[GNU GPL 3.0](https://choosealicense.com/licenses/gpl-3.0/)
