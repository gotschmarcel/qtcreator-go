#include "gofile.h"

#include <utils/fileutils.h>
#include <coreplugin/id.h>

#include "../goconstants.h"

using namespace Go::Internal;

GoFile::GoFile(const Utils::FileName& fileName)
{
    setId("Go.ProjectFile");
    setMimeType(QLatin1String(Constants::MIMEType));
    setFilePath(fileName);
}

Core::IDocument::ReloadBehavior GoFile::reloadBehavior(ChangeTrigger, ChangeType) const
{
    return BehaviorAsk;
}
