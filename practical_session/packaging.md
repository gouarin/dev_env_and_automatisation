# Création de version et packaging

Nous voici dans la dernière partie de cet atelier. Nous souhaitons à présent pouvoir déclencher un événement à la main qui

- crée un `CHANGELOG` à partir des commits réalisés depuis la dernière version,
- met à jour la version,
- crée une archive de cette nouvelle version,
- lance la construction d'un paquet `conda` et dépose celui-ci dans votre `channel`.

:::{note}
N'oubliez pas de retourner dans la branche `main` et de faire

```bash
git pull
```
:::

Jusqu'à présent, nous n'avons pas fait attention si le nom de la pull request satisfaisait aux critères du [commit conventionnel](#conventional-commits). Hors, cela devient ici indispensable pour que le `CHANGELOG` soit généré correctement et automatiquement. Nous allons donc commencer par mettre une action en place qui vérifie que le titre de la pull request suit la nomenclature du commit conventionnel.

## Première version

Afin d'automatiser le processus de création de version de manière automatique, nous devons l'initier. Pour cela, nous allons créer un fichier `version.txt` à la racine du projet avec le contenu suivant

```text
0.0.1
```

Une fois que cette modification est fusionnée dans la branche principale, il suffit de créer une première version du projet avec le même numéro de version.

Allez sur votre dépôt GitHub et cliquez sur `Release` qui se trouve dans la colonne de droite. Puis `Create a new release`.

Allez dans `Choose a tag` et rentrez `v0.0.1`. Et enfin `Publish release`.

En retournant sur la page d'accueil de votre dépôt, vous devriez voir apparaître en dessous de `Release` votre `v0.0.1`.

## Vérification du titre de la pull request

Nous allons à présent vérifier le titre de la pull request. Tout se passe encore une fois dans la section `on` qui gère les événements. Nous allons à présent utiliser l'événement [`pull_request_target`](https://docs.github.com/actions/using-workflows/events-that-trigger-workflows#pull_request_target) qui intervient à chaque fois qu'une opération est demandée sur l'interface de la pull request.

Nous allons sélectionner trois types d'activités

- `opened`: lorsque la pull request est créée
- `edited`: lorsque le titre de la pull request est modifiée
- `synchronize`

Nous utiliserons l'action [semantic-pull-request](https://github.com/marketplace/actions/semantic-pull-request) pour la validation.

Créez l'action associée, testez et fusionnez dans la branche principale.

## Création automatique d'une nouvelle version

À présent, à partir des titres de la pull request, nous allons pouvoir déterminer les versions successives en utilisant la gestion sémantique de version [semver](https://semver.org). Nous n'allons pas rentrer dans les détails, mais donner juste assez d'éléments pour la compréhension de la suite. Un numéro de version peut être décomposé comme suit

:::{figure} ./figures/semver.png
:::

La version corrective correspond à une correction qui corrige un comportement interne incorrect, mais qui ne casse pas l'API actuelle. La version mineure correspond à l'ajout de nouvelles fonctionnalités qui ne cassent pas l'API actuelle. Enfin, la version majeure correspond à des changements qui cassent l'API actuelle.

Généralement, lorsque la version majeure est supérieure ou égale à `1`, le logiciel sort d'un état de développement pour aller vers un état dit stable. Notre projet est un problème jouet et ne peut être considéré comme un logiciel dans un état stable. Nous garderons donc la version majeure à `0`. Par conséquent, si la version mineure est augmentée, cela signifie que des nouvelles fonctionnalités ont été ajoutées et que celles-ci peuvent casser ou non l'API actuelle.

Revenons aux commits conventionnels et à leur influence sur la montée de version.

- Si depuis la dernière version, l'ensemble des commits sont du type `fix`, alors seule la version corrective sera incrémentée.
- Si depuis la dernière version, au moins un des commits est de type `feat`, alors la version mineure est incrémentée et la version corrective est remise à zéro.

Comment générer ce modèle de manière automatique sur notre dépôt ? En utilisant GitHub Actions !

Nous allons utiliser cette action : [release-please-action](https://github.com/marketplace/actions/release-please-action) qui utilise [release-please](https://github.com/googleapis/release-please).

`release-please` a besoin de fichiers de configuration pour contrôler son comportement. Voici les deux fichiers à ajouter à la racine du dépôt

- `release-please-config.json`
    ```json
    {
        "release-type": "simple",
        "include-component-in-tag": false,
        "include-v-in-tag": true,
        "changelog-path": "CHANGELOG.md",
        "bump-minor-pre-major": true
    }
    ```

- `.release-please-manifest.json`
    ```json
    {
        ".": "0.0.1"
    }
    ```

:::{tip} `release-please-config.json`
Si vous voulez comprendre ce que fait ce fichier de configuration, il vous suffit de vous rendre à [Manifest Driven release-please](https://github.com/googleapis/release-please/blob/main/docs/manifest-releaser.md).
:::

:::{tip} Type de déclenchement
Nous déclencherons l'action à la main. Par conséquent

```yaml
on: workflow_dispatch
```
:::

Cette action se joue en deux temps.
- La première fois que vous la déclenchez, elle ouvre une pull request avec la version qui sera créée et le `CHANGELOG.md`.
- La deuxième fois que vous la déclenchez, elle crée la nouvelle version dans les `Release` de votre dépôt GitHub.

::::{important}
Il n'est pas nécessaire de mettre un `token`.

En revanche, la création de la pull request nécessite que GitHub Actions ait des droits sur votre dépôt. Il faut aller dans `Settings->Actions->General` et cocher la case tout en bas

:::{figure} ./figures/action-perm.png
:::
::::

Créez l'action associée et ajoutez les fichiers de configuration à votre dépôt. Vous pouvez fusionner à la branche principale.

D'ici, vous devriez avoir deux commits depuis la dernière version et ceux-ci devraient suivre la nomenclature des commits conventionnels. Vous pouvez donc tester la création d'une nouvelle version !

Vous pouvez également tester en faisant des pull request qui mixent des `fix` et des `feat` et voir le comportement.

## Packaging conda

Nous allons montrer dans cette section comment déclencher la construction d'un package conda lorsqu'une nouvelle version est disponible et que ce package soit directement accessible sur votre `channel` conda.

:::{attention}
Nous faisons cela à titre d'exemple. Si vous voulez créer un package conda et le distribuer, il est préférable de le mettre sur [conda-forge](https://conda-forge.org/docs/maintainer/adding_pkgs.html) pour plus de visibilité.
:::

### Construction locale

La construction d'un package conda se fait en créant une recette. Celle-ci est dans le répertoire `6.packaging/required_file/recipe`. Ce dossier est à mettre à la racine de votre dépôt. Il contient

- Un fichier `meta.yaml` décrivant la version du paquet, où trouver le paquet, quelles sont les dépendances au moment du build et du runtime...
- Un fichier `build.sh` qui décrit comment construire le paquet sous linux et macos.
- Un fichier `bld.bat` qui décrit comment construire le paquet sous windows.

Nous allons dans un premier temps tester localement la construction de notre package. Pour cela, il vous faut installer `conda-build` dans `pixi`. Une fois que c'est fait, éditez le fichier `meta.yaml` et changez la version en fonction de ce que vous avez sur votre dépôt et le champ `git_url` afin de mettre votre adresse de dépôt.

Il faut suffit ensuite de faire

```bash
conda build recipe
```

Si tout s'est bien passé, vous devriez voir où est stocké le package.

### Anaconda

Nous allons maintenant essayer de mettre le package construit localement sur [anaconda](https://anaconda.org/) pour être sûr que tout le processus fonctionne avant de l'automatiser. Vous devez donc vous créer un compte pour créer votre propre `channel`.

Une fois le compte créé, vous pouvez mettre votre paquet sur votre compte. Il vous faut installer `anaconda-client` via `pixi`, puis

```bash
anaconda upload /path_to/splinart-*.tar.bz2
```

Si tout s'est bien passé, vous devriez voir votre paquet sur votre compte anaconda. Si vous voulez tester l'installation, éditez votre fichier `pixi.toml` et ajoutez dans les channels à côté de `conda-forge` le nom de votre `channel`.

