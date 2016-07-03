#include "goprojectfile.h"

#include <coreplugin/id.h>
#include <utils/fileutils.h>

#include "../goconstants.h"

using namespace Go::Internal;

GoProjectFile::GoProjectFile(const Utils::FileName& fileName) {
    setId("Go.ProjectFile");
    setMimeType(QLatin1String(Constants::MIMEType));
    setFilePath(fileName);
}

Core::IDocument::ReloadBehavior GoProjectFile::reloadBehavior(ChangeTrigger, ChangeType) const {
    return BehaviorAsk;
}
