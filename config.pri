# Qt Creator linking

## Either set the IDE_SOURCE_TREE when running qmake,
## or set the QTC_SOURCE environment variable, to override the default setting
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = $$(QTC_SOURCE)
isEmpty(IDE_SOURCE_TREE) {
    error("Either set the IDE_SOURCE_TREE when running qmake or set the QTC_SOURCE environemnt variable.")
}

## Either set the IDE_BUILD_TREE when running qmake,
## or set the QTC_BUILD environment variable, to override the default setting
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE) {
    error("Either set the IDE_SOURCE_TREE when running qmake or set the QTC_SOURCE environemnt variable.")
}
