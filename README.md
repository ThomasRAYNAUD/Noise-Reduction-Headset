# Noise-Reduction-Headset
> Le but du projet est d'implémenter en C++ l'algorithme de traitement audio LMS sur un teensy arduino 4.0 permettant de réduire le bruit ambiant. Pour cela, on fait appel à la librairie MyDsp de teensy qui permet d'échantilloner des signaux audio. Le traitement s'éffectuera sur chacun des échantillons.
> La version actuelle fait appel à un bruit blanc généré en interne par le teensy et qu'il va chercher à supprimer avec l'algorithme. Ce programme peut servir de base pour l'implémentation d'un système haut-parleur/microphone en tant que source de bruit.



## Resources : 

- https://github.com/zftan0709/Feedback-ANC-Teensy-3.6

- https://www.researchgate.net/publication/333567723_Active_Noise_Reduction_using_LMS_and_FxLMS_Algorithms

- https://iopscience.iop.org/article/10.1088/1742-6596/1228/1/012064/pdf

- convergence FxLMS : https://www.sciencedirect.com/science/article/pii/S2212017316304741

- somme de signaux : https://www.researchgate.net/figure/Delayless-sub-band-ANC-system-using-the-FXLMS-algorithm_fig3_235923183
  (voix + bruit. Obj -> filtrer ce bruit quand on appuie sur le bouton.)

- https://www.researchgate.net/publication/264337037_Active_Noise_Control_Basic_Understanding

- 
## Vocabulaire

- **ANR** : Active Noise Reduction.
- **PNR** : Passive Noise Reduction.
- **LMS** : Least Mean Square.
- **FxLMS** : Filtered-x Least Mean Square.
