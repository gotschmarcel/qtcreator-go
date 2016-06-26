#include "goeditorfactory.h"

#include <QCoreApplication>
#include <texteditor/textdocument.h>
#include <texteditor/indenter.h>
#include <texteditor/texteditoractionhandler.h>

#include "../goconstants.h"
#include "goeditor.h"
#include "goeditorwidget.h"
#include "gosyntaxhighlighter.h"

using namespace Go;

EditorFactory::EditorFactory()
{
    setId(Constants::EditorID);
    setDisplayName(qApp->translate("OpenWith::Editors", Constants::EditorDisplayName));
    addMimeType(Constants::MIMEType);
    addMimeType(Constants::ProjectMIMEType);

    setDocumentCreator([]() { return new TextEditor::TextDocument(Constants::EditorID); });
    setIndenterCreator([]() { return new TextEditor::Indenter; });
    setEditorWidgetCreator([]() { return new EditorWidget; });
//    setAutoCompleterCreator([]() { return new AutoCompleter; }});
//    setCompletionAssistProvider([]() { return new CompletionAssistProvider; });
    setSyntaxHighlighterCreator([]() { return new SyntaxHighlighter; });
    setCommentStyle(Utils::CommentDefinition::CppStyle);
    setParenthesesMatchingEnabled(true);
    setCodeFoldingSupported(true);
    setMarksVisible(true);

//    addHoverHandler(new HoverHandler);

    setEditorActionHandlers(TextEditor::TextEditorActionHandler::Format
                            | TextEditor::TextEditorActionHandler::UnCommentSelection
                            | TextEditor::TextEditorActionHandler::UnCollapseAll
                            | TextEditor::TextEditorActionHandler::FollowSymbolUnderCursor);
}
