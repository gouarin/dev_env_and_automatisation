# Première action GitHub

:::{hint}Objectifs de cette partie
- Mettre en place l'intégration continue
- Réaliser des tests unitaires du projet sur différents OS
:::

:::{note}Notions GitHub Actions abordées
- Où mettre les actions ?
- Fonctionnement d'un workflow
- Mise en place d'une matrice
:::

Nous allons ici mettre en place la première action à exécuter par GitHub Actions. Nous allons voir si ce que nous avons réussi à faire fonctionner localement avec pixi fonctionne également dans l'intégration continue.

Vous allez commencer par créer une branche, car, rappelez-vous il est interdit de mettre des choses directement dans la branche principale. Nous voulons nous assurer que les changements soient le plus sain possible par rapport à notre cadre de développement.


## Premier workflow

Allons-y, créons cette nouvelle branche

```bash
git checkout -b first-action
```

Nous allons maintenant créer le fichier `.github/workflows/ci.yml` contenant

```yaml
name: "Run CI"

on: pull_request

jobs:
  tests:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

```

Ajoutez ce fichier via un commit et pousser la branche sur votre dépôt GitHub.

Vous devriez être en mesure d'ouvrir une pull request à partir de cette branche. Une fois que c'est fait, vous devriez voir l'action se mettre en place, car nous avons demandé qu'elle se déclenche à chaque pull request. Pour plus de détail, vous pouvez par ailleurs aller dans l'onglet `Actions`.

## Mise en place des tests

Si tout s'est bien passé, ajoutez l'étape suivante

```yaml
- run: pixi run test
```

Ajoutez ces changements dans la pull request : il suffit de commiter ces changements localement et de les pousser.

L'intégration continue devrait échouer, car la commande `pixi` n'est pas reconnue. Il vous faut donc ajouter une étape : l'installation de pixi.

Vous pourriez le faire à la main, mais il vous faut savoir comment différencier l'installation entre OS et vous ne savez pas encore comment faire. Vous vous dites que vous allez peut-être trouver votre bonheur dans le [MarketPlace](https://github.com/marketplace?type=actions).

Recherchez s'il existe une action pour installer pixi et si oui, ajoutez cette étape à votre processus.

Vous devriez maintenant avoir une action qui construit et teste l'application sous ubuntu.

## Multi OS

Tout a l'air de fonctionner sous linux, mais en est-il de même sur les autres OS. Pour s'en assurer, nous décidons d'ajouter macos.

Pour cela, nous allons utiliser la notion de matrice dans GitHub Actions qui permet de lancer l'ensemble des étapes pour différentes configurations.

Prenons l'exemple suivant

```yaml
jobs:
  python_tests:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest]
        py-version:
          - "3.10"
          - 3.11
          - 3.12
    steps:
      - name: Set up Python ${{ matrix.py-version }}
        uses: actions/setup-python@v2
        with:
          python-version: ${{ matrix.py-version }}
```

La matrice est décrite dans la partie `strategy` grâce au mot clé `matrix`. Les travaux qui seront exécuter sont les suivants

- {os: ubuntu-latest, py-version: 3.10}
- {os: macos-latest, py-version: 3.10}
- {os: ubuntu-latest, py-version: 3.11}
- {os: macos-latest, py-version: 3.11}
- {os: ubuntu-latest, py-version: 3.12}
- {os: macos-latest, py-version: 3.12}

:::{note} Python 3.10
Il est nécessaire de mettre 3.10 entre guillemets, car sinon l'intégration continue enlève le 0 et cherche la version Python 3.1.
:::

Pour accéder aux éléments de la matrice, il suffit de faire `matrix.nom`. Vous pouvez définir la liste entre `[]` comme pour l'entrée `os` ou sous forme de tirets comme pour `py-version`.

Modifiez votre workflow pour que celui-ci soit également testé sur macos.

Une fois que tout fonctionne, vous pouvez faire un `Squash and merge`.