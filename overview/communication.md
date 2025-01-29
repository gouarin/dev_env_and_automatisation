# La communication

:::{hint}Objectifs de cette partie
- Identifier les acteurs
- Mieux cerner les différentes formes de communications
- Comprendre leurs impacts sur un projet open source
:::

La communication est un élément essentiel pour que le projet prenne de l'ampleur et qu'une communauté se crée. Elle doit tenir compte de chacune des personnes susceptibles d'apporter sa contribution. Celle-ci peut-être de différentes natures : indiquer un bug, demander une nouvelle fonctionnalité, poser une question, corriger un bug, faire de la documentation, ajouter une nouvelle fonctionnalité, relire les pull requests et les valider, ...

Comment faire en sorte que l'ensemble de ces étapes qui sont inhérentes à chaque développement open source se passent au mieux ? Comment faire pour donner une certaine autonomie aux acteurs pour qu'ils se sentent investis et créatifs dans un cadre contraint ?

Si nous y réfléchissons un peu, nous n'aimons généralement pas ne pas savoir par où commencer surtout lorsque nous prenons le travail de quelqu'un d'autre. Malgré la persévérance de certain, il n'est pas rare de voir des acteurs potentiels se décourager et aller voir chez quelqu'un d'autre. Mais, si ça se trouve, il ne manquait pas grand-chose : une documentation, une information, ...

Nous allons voir dans la suite différents documents et outils qui permettent aux acteurs de se sentir bien et en confiance. Ils savent qu'ils trouveront la réponse à leurs interrogations et, si ce n'est pas le cas, ils sauront où s'adresser pour l'avoir.

:::{attention}La langue
Si vous voulez toucher le plus de monde possible, il est fortement recommandé d'écrire en anglais : les commentaires dans le code, la documentation, les messages des commits, ...
:::

## Qui a le droit d'utiliser ce logiciel ?

Pour que votre logiciel open source puisse être utilisé, il doit contenir une licence qui décrit dans quelles conditions celui-ci peut être utilisé. Le choix d'une licence est un vaste sujet et dépasse largement le cadre de cet atelier. Nous ne donnerons ici que quelques pointeurs qui vous permettront d'approfondir la question puis nous expliquerons la mise en œuvre dans le dépôt GitHub.

:::{tip} Ressources aidant au choix d'une licence
- [Liste des licences](https://opensource.org/licenses/)
- [The Legal Side of Open Source](https://opensource.guide//legal/)
- [Science ouverte : codes et logiciels](https://www.ouvrirlascience.fr/wp-content/uploads/2022/10/Passeport_Codes-et-logiciels_WEB.pdf)
- [The Software Package Data Exchange (SPDX)](https://spdx.dev/)
:::

Nous rappelons également qu'il est important de se rapprocher de son employeur et de le consulter pour la mise en ligne d'un projet open source.

Pour ajouter une licence à votre dépôt, il vous suffit d'ajouter un fichier `LICENSE` ou `LICENSE.md` qui contient le contenu légal de la licence que vous aurez choisie. Prenons par exemple le contenu de la licence MIT qui est assez court

> Copyright `<YEAR> <COPYRIGHT HOLDER>`
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Vous pouvez le faire manuellement : c'est-à-dire créer le fichier localement, ouvrir une pull request et enfin insérer celle-ci dans la branche principale. Vous pouvez également le faire au travers de l'interface de GitHub [Adding a license to a repository](https://docs.GitHub.com/en/communities/setting-up-your-project-for-healthy-contributions/adding-a-license-to-a-repository).

Il est recommandé de mettre en en-tête de fichier la licence ainsi que le copyright. Par exemple, dans un fichier C++

```cpp
// SPDX-License-Identifier: MIT
// Copyright 2023 FOO TEAM. All rights reserved.
```

## Que fait ce logiciel ?

Lorsque vous avez besoin de savoir si un logiciel existe pour vos besoins, que faites-vous ? Probablement que vous allez faire une recherche sur votre moteur de recherche préféré, il se peut aussi que vous alliez directement sur GitHub pour taper quelques mots clés et voir si vous trouvez votre bonheur. Quoi qu'il en soit, la première page que vous allez trouver sera certainement la page d'un dépôt GitHub ou GitLab. Il est donc important que celle-ci donne assez de renseignements pour maintenir la lectrice ou le lecteur en haleine.

Sur GitHub, les informations affichées sur un dépôt sont celles contenues dans le fichier `readme`.

:::{note}
Un fichier de `readme` peut avoir plusieurs extensions `.md`, `.rst` ou sans extension.
:::

Si c'est la première vue de votre logiciel, vous comprendrez bien qu'il est important de ne pas en négliger sa rédaction. Il existe différents sites qui vous aident sur l'élaboration de son contenu.

:::{tip}Ressources pour écrire un `readme`
- [Make a README](https://www.makeareadme.com/)
- [Awesome README](https://GitHub.com/matiassingers/awesome-readme)
- [GitHub README Templates](https://www.readme-templates.com/)
:::

Voici quelques éléments qui devraient faire partie de votre `readme`:

- Le nom du projet
- Une description du projet
- Des badges permettant d'accéder facilement à la documentation, aux tests, aux packages, ...
- Un guide sur l'installation
- Un exemple d'utilisation
- Où aller pour plus d'informations
- Comment contribuer
- Une feuille de route du projet (où nous souhaitons aller)

:::{attention}
Le `readme` n'est pas la documentation de votre logiciel. Il ne doit donc pas être trop long, mais assez complet pour trouver de l'information rapidement.

:::

## Comment installer ce logiciel ?

Il est plus facile d'essayer un logiciel lorsque nous n'avons rien à faire. S'il faut installer dix mille dépendances qui ne sont même pas documentées et que, même après cette étape, vous n'arrivez pas à installer le logiciel, car il y a un problème de version, vous allez aller voir ailleurs et c'est bien dommage pour celui qui a développé ce logiciel génial, mais qui n'a pas pris le temps de prendre soin du packaging.

Si l'installation est bien documentée (même si c'est à partir des sources), que le processus n'a pas l'air trop contraignant, alors il y a de forte chance que la personne essaie. Bien évidemment, le logiciel évolue et les dépendances avec lui. Il serait bon que cette partie soit testée. C'est ce que nous ferons par la suite en utilisant GitHub Actions.

Nous pouvons aller encore plus loin en ayant le logiciel sur un système de gestion de paquets (yarn, cargo, conda, ...). La personne n'a plus qu'à utiliser sa ligne de commande préférée pour installer le logiciel.

Il existe également des solutions où il n'y a qu'à cliquer sur une url et se laisser guider. Parmi les outils qui existent, nous pouvons citer

- [solara](https://solara.dev/)
- [streamlit](https://streamlit.io/)
- [voilà](https://voila-gallery.org/)

Ils permettent de développer rapidement des tableaux de bord à l'aide de notebook Jupyter ou autres où nous pouvons tester le logiciel. Le déploiement se fait directement sur des services cloud (streamlit, [binder](https://mybinder.org/), [Ploomber Cloud](https://www.platform.ploomber.io/), [netlify](https://www.netlify.com/),...). Il est de plus possible d'avoir accès à un terminal avec le logiciel installé et les exemples compilés.

## Comment se servir de ce logiciel ?

La deuxième étape (une fois que la personne veut en savoir un peu plus) repose sur la documentation. Il existe différentes formes de documentation à destination d'acteurs différents : un utilisateur débutant, un utilisateur confirmé, un contributeur, un relecteur, ... Mais il est néanmoins préférable d'avoir un unique espace où trouver celle-ci. Avant de parler des outils, revenons sur son contenu. Vous avez une très bonne ressource qui vous donne un tas d'informations sur la création de la documentation d'un logiciel : [Write the Docs](https://www.writethedocs.org/guide/). Vous avez également ce très bon livre [Docs for Developpers](https://docsfordevelopers.com/).

Les différentes parties de la documentation peuvent se résumer ainsi (elles se trouvent dans le livre cité précédemment)

- Une partie **débuter** (*getting started*) qui permet de se faire une première impression.
- Une **documentation conceptuelle** qui explique les idées et les concepts du logiciel.
- Une **documentation procédurale** qui contient
  - des **tutoriels** qui expliquent comment résoudre un problème à l'aide du logiciel,
  - des **comment faire ?** (*how-to*) qui expliquent les différentes étapes à réaliser pour atteindre un objectif.
- Une **documentation de référence** qui contient
  - une documentation de l'**API**
  - une documentation sur la **résolution des problèmes**
  - une documentation sur **comment contribuer**

Pour faciliter l'écriture et rendre disponible cette documentation, vous avez à votre disposition de nombreux outils. En calcul scientifique, [sphinx](https://www.sphinx-doc.org) est très utilisé pour l'écriture et la génération de la documentation en différents formats (html, pdf, epub, ...). Il peut faire le lien avec les docstrings de Python, mais également la documentation de l'API réalisée avec [doxygen](https://www.doxygen.nl/) au travers de l'extension [breathe](https://breathe.readthedocs.io). L'écriture des autres documentations peut se faire en utilisant des fichiers markdowns, reStructuredText ou des notebooks Jupyter.

:::{tip}MyST
MyST permet d'écrire en markdown tout en offrant une flexibilité que le format reStructuredText offre. Ce document est d'ailleurs rédigé en l'utilisant. Pour plus d'information https://mystmd.org.
:::

Une fois que la documentation est bien générée localement, vous pouvez la rendre disponible en ligne en utilisant [Read the Docs](https://readthedocs.org) ou [GitHub Pages](https://pages.GitHub.com/). Nous verrons la deuxième possibilité en utilisant GitHub Actions.

Nous avons vu jusqu'à présent comment, à partir d'un ensemble de documents, orienter les acteurs au mieux. Mais, que ce passe t-il s'ils n'ont pas trouvé ce qu'ils cherchaient ? Comment peuvent-ils rentrer en contact avec vous et échanger ? C'est l'objet des trois prochaines parties.

## Les règles de bonne conduite

Avant tout échange, il est bon de rappeler comment vous devez vous comporter et comment les autres personnes doivent également se comporter. Offrir un espace bienveillant et sécurisant est important pour ses contributeurs et sa communauté. Pour ce faire, la plupart des dépôts sur GitHub ayant une forte audience ont un code de conduite (*code of conduct*).

:::{tip} Code of conduct
Pour plus d'informations et avoir un modèle dans différents formats : https://www.contributor-covenant.org/.
:::

## Une question, un problème ?

Un utilisateur trouve un bug dans le logiciel et souhaite vous en avertir. Il peut bien évidemment ouvrir une issue sur GitHub mais que doit-il écrire ? Sera-t-il assez clair pour que vous ne deviez pas échanger une dizaine de messages avant de comprendre de quoi il s'agit ?

Souhaitez-vous que les questions en tout genre, la demande d'ajout de nouvelles fonctionnalités se retrouvent toutes dans les issues de votre dépôt ? Si les demandes de nouvelles fonctionnalités ou les rapports de bug sont éphémères : une fois que c'est fait, on n'en parle plus. Certaines questions sont intéressantes à garder. Rappelons que lorsque l'issue est fermée, vous ne la voyez plus sur votre dépôt sauf si vous allez dans celles qui sont fermées.

La question est donc : est-ce que l'on peut orienter les acteurs pour qu'ils prennent les bons chemins en cas de problème ?

GitHub offre la possibilité de mettre en place des templates pour les issues et pour les pull requests. Nous ne verrons ici que la partie issue, car elle est plus flexible (pour le moment) que la partie pull request. En effet, vous pouvez créer des formulaires en utilisant des fichiers `yaml`. Vous pouvez trouver les informations sur [Configuring issue templates for your repository](https://docs.GitHub.com/en/communities/using-templates-to-encourage-useful-issues-and-pull-requests/configuring-issue-templates-for-your-repository).

Voici un exemple de ce qui peut être mis en place lorsqu'une personne clique sur l'onglet `issue` de votre dépôt

```{image} figures/issue_template.png
:align: center
```

Et si elle clique sur `Bug Report`, elle se retrouve devant un formulaire de ce type

```{image} figures/bug_report.png
:align: center
```

Il est important de noter que nous avons mis un cadre : nous essayons d'aiguiller la personne le plus possible afin qu'elle se sente à l'aise pour trouver son chemin et qu'elle sache comment initier un dialogue avec vous en fournissant un certain nombre d'éléments. Vous avez gagné un temps précieux et elle aussi.

## Une contribution ?

Enfin, il est également important de savoir comment contribuer au projet, quel est le cadre. Contribuer à un projet prend du temps et il est donc primordial de perdre le moins de temps possible pour s'approprier la structure globale du projet, sa philosophie de développement et son fonctionnement. Pour cela, vous pouvez ajouter un fichier `CONTRIBUTING.md` dans lequel vous pouvez mettre

- un message de bienvenue
- une table des matières si le document est trop grand
- des informations sur comment mettre en place l'environnement de développement
- une description sur la mise en place des tests et comment en écrire d'autres
- un protocole pour soumettre sa contribution
- les façons de contribuer autres que du code
- ...

Voici un exemple : le fichier `CONTRIBUTING.md` de scikit-learn

```{image} figures/scikit-learn-contributing.png
:align: center
```

:::{tip} Templates
Si vous recherchez des templates pour toutes les étapes introduites dans ce chapitre ou pour d'autres choses en lien avec la documentation d'un projet open source, vous pouvez aller voir [The Good Docs Project](https://thegooddocsproject.dev/) et ses [templates](https://gitlab.com/tgdp/templates).
:::

<!-- GitHub discussion -->
