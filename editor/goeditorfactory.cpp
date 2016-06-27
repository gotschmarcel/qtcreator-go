#include "goeditorfactory.h"

#include <QCoreApplication>
#include <texteditor/textdocument.h>
#include <texteditor/indenter.h>
#include <texteditor/texteditoractionhandler.h>
#include <texteditor/codestylepool.h>

#include "../goconstants.h"
#include "goeditor.h"
#include "goeditorwidget.h"
#include "gosyntaxhighlighter.h"
#include "goindenter.h"

using namespace Go;

EditorFactory::EditorFactory()
{
    setId(Constants::EditorID);
    setDisplayName(qApp->translate("OpenWith::Editors", Constants::EditorDisplayName));
    addMimeType(Constants::MIMEType);
    addMimeType(Constants::ProjectMIMEType);

    setDocumentCreator([]() {
        auto doc = new TextEditor::TextDocument(Constants::EditorID);
        doc->setMimeType(QLatin1String(Constants::MIMEType));
        return doc;
    });

    setIndenterCreator([]() { return new Indenter; });
    setSyntaxHighlighterCreator([]() { return new SyntaxHighlighter; });

    setEditorCreator([]() { return new Editor; });
    setEditorWidgetCreator([]() { return new EditorWidget; });

//    setAutoCompleterCreator([]() { return new AutoCompleter; }});
//    setCompletionAssistProvider([]() { return new CompletionAssistProvider; });

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
