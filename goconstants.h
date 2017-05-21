#pragma once

namespace Go {
namespace Constants {

const char EditorID[]           = "Go.Editor";
const char ProjectID[]          = "Go.Project";
const char Icon[]               = ":/go/gopher.png";
const char SettingsID[]         = "Go.Settings";
const char SettingsBuildNRunID[] = "Z.GoBuildNRun";
const char SettingsName[]       = "Go";
const char SettingsCategory[]   = "I.Go";
const char SettingsDisplayCategory[] = "Go";
const char EditorDisplayName[]  = "Go Editor";
const char LangGo[]             = "GO";

const char MIMEType[]           = "application/x-go";
const char ProjectMIMEType[]    = "text/x-gocreator-project";

const char ProjectFileExt[]     = ".goproject";
const char FileExt[]            = "go";

const char GoPathEnvName[]      = "GOPATH";
const char GoRootEnvName[]      = "GOROOT";
const char GoExecName[]         = "go";

const char MessagePrefix[]      = "[Go Plugin] ";

const char ProjectContext[]     = "GoProjectContext";

// Toolchain
#if defined(Q_OS_WIN32)
static const QString DefaultGoRoot(QLatin1String("C:\\go\\bin"));
#elif defined(Q_OS_MAC) || defined(Q_OS_LINUX)
static const QString DefaultGoRoot(QLatin1String("/usr/local/go/bin"));
#else
#error "Unsupported OS"
#endif

} // namespace Go
} // namespace Constants
