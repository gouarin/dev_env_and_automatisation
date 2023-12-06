# GitHub Actions

:::{hint}Objectifs de cette partie
- Tour d'horizon des outils d'intégration continue
- Présentation de GitHub Actions
:::

Afin d'automatiser les tâches qui peuvent l'être dans le cadre du développement de logiciel open source, nous allons nous servir de l'intégration continue que nous trouvons sur GitHub. L'intérêt étant de fixer le cadre, de le rendre lisible et de vérifier que personne ne sort de celui-ci. Ce n'est donc plus à vous de tout vérifier, mais aux contributeurs de faire en sorte que l'ensemble du processus soit correct.

Il existe un certain nombre d'outils d'intégration continue. Sur GitHub, vous avez entre autres [GitHub Actions](https://github.com/features/actions), [azure-pipeline](https://azure.microsoft.com/fr-fr/products/devops/pipelines), [circle-ci](https://circleci.com/), [travis CI](https://www.travis-ci.com/), [appveyor](https://www.appveyor.com/). Tous ces services sont directement accessibles et utilisables sur des dépôts publics. Ils proposent différents OS pour tester son application.

Travis CI jouissait d'une grande notoriété et était le système d'intégration continue le plus utilisé sur GitHub. Malheureusement, suite à des problèmes de qualité de service répétés et à un changement de la politique d'utilisation gratuite de la plateforme pour des projets open source, cet outil a perdu de nombreux utilisateurs. L'arrivée de GitHub Actions fin 2019 n'a rien arrangé. Aujourd'hui, GitHub Actions est le système d'intégration continue le plus utilisé sur GitHub.

Nous parlerons dans la suite uniquement de GitHub Actions, car il offre des fonctionnalités très intéressantes que les autres systèmes n'ont pas. En effet, il permet de créer n'importe quelle action dans GitHub. Par exemple, vous pouvez déclencher une action lors de la création d'une pull request (ce que font les autres outils externes cités précédemment). Mais, il y a mieux : vous pouvez créer une issue si la pull request ne se passe pas bien, vous pouvez vérifier le nom de la pull request, vous pouvez déclencher une action lors de la création d'une issue, ... Bref, vous pouvez définir finement ce que vous voulez faire dans votre dépôt, quand le faire et comment.

:::{note} GitLab CI/CD
Nous aurions pu également parler de GitLab et de son outil d'intégration continue [GitLab CI/CD](https://docs.gitlab.com/ee/ci/). Vous pouvez vraisemblablement faire exactement la même chose que ce qui sera présenté dans ce atelier avec cet outil. Néanmoins, vous devrez reproduire dans le fichier `gitlab-ci.yml` l'ensemble des étapes réalisées par chacune des actions.

Vous trouverez plus d'informations ici: https://docs.gitlab.com/ee/ci/migration/github_actions.html
:::

:::{note} act
Vous avez l'outil [act](https://github.com/nektos/act) qui vous permet de tester localement votre processus défini par GitHub Actions. Malheureusement, cet outil ne fonctionne pour le moment que pour réaliser des actions sous linux via une image docker. Mais, il est normalement destiné à fonctionner aussi sur macos et windows.
:::

## Comment ça marche ?

Les actions que vous allez créer sont décrites dans des fichiers `yaml` et doivent se trouver dans le répertoire `.github/workflows/` de votre dépôt. Chacune d'elles est composée

- d'une définition de ou des événements GitHub qui vont déclencher l'action
- d'une liste de travaux à réaliser

Pour chacun des travaux, vous pouvez indiquer sur quels OS vous souhaitez exécuter le travail suivi d'une liste de tâches à effectuer. Il vous est possible d'ajouter des contraintes sur ces différents travaux : par exemple, un travail ne s'exécute que si d'autres travaux ont été effectués correctement.

Prenons un exemple très simple

``` yaml
name: "Print hello"

on: pull_request

jobs:
  print-hello:
    runs-on: ubuntu-latest
    steps:
      - run: echo "Hello !!!"

```

Reprenons ces quelques lignes

- `name` permet de donner un nom à votre action. Si vous ne le faites pas, le nom de l'action sera le nom du fichier.
- `on` décrit à quel moment l'action doit être déclenchée. Ici, ce sera au moment d'une pull request: ouverture ou modification.
- Nous avons ensuite la liste des travaux à effectuer. Ici, il n'y en a qu'un qui s'appelle `print-hello`.
  - `runs-on` définit sur quel OS le travail va être exécuté. Nous avons choisi ici ubuntu, mais vous en avez d'autres comme nous le verrons dans la suite.
  - `steps` définit les étapes du travail. Il n'y a ici qu'une étape utilisant `run` qui indique une ligne de commande à exécuter dans un shell.

:::{note}
Il ne sera pas nécessaire d'essayer cette action. Elle est juste montrée à titre d'exemple. Nous ferons des actions bien complexes et utiles dans la suite.
:::

## L'outil en ligne

Nous avons montré comment créer une action en écrivant le fichier `yaml`. Sachez qu'il vous est possible de le faire via l'interface de GitHub. Cela peut être pratique au début pour se familiariser avec l'outil.

Voici comment faire

:::{figure} ./figures/create-github-actions.mp4
Exemple d'utilisation de l'interface GitHub pour créer une action.
:::

## Où trouver des actions ?

Nous avons montré en exemple une action très simple qui exécutait une ligne de commande. Il est possible d'utiliser des actions écrites par des tierces personnes et mises à disposition pour la communauté. Par exemple, nous allons souvent utiliser celle-ci

```yaml
    ...
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

```

qui va s'occuper de cloner le dépôt sur la machine cible. C'est ce qui fait la force de GitHub Actions : vous pouvez trouver des recettes pour faire plein de choses  (installer son environnement Python, utiliser pixi, construire un artifact, créer une nouvelle version, mettre votre logiciel sur PyPi...)

Le [MarketPlace](https://github.com/marketplace?type=actions) est l'endroit où trouver tout type d'actions. Voici par exemple la page de l'action `checkout`

:::{figure} ./figures/checkout.png
:::

Chaque action a généralement une documentation qui explique comment l'utiliser. Vous avez également un système d'étoiles comme sur GitHub qui vous permet de savoir si cette action est appréciée par la communauté.

:::{note} Besoin d'aide
Si vous avez besoin d'aide, le site GitHub qui y est consacré est très bien fait https://docs.github.com//actions.

Mais, nous devrions voir dans la suite assez de cas d'usages pour vous permettre de bien débuter.
:::
