package io.agora.liveshop.data;

import java.util.List;

public class Quiz {
    private int id;
    private String content;
    private List<String> options;
    private int timeout;
    private int total;
    private QuizResult result;

    @Override
    public int hashCode() {
        return id + content.hashCode() + result.hashCode();
    }

    @Override
    public boolean equals(Object object) {
        if (!(object instanceof Quiz)) {
            return false;
        }

        Quiz quiz = (Quiz) object;
        return this.id == quiz.id && this.content.equals(quiz.content) &&
                this.result.equals(quiz.result);
    }

    public QuizResult getResult() {
        return result;
    }

    public void setResult(QuizResult result) {
        this.result = result;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public int getTotal() {
        return total;
    }

    public void setTotal(int total) {
        this.total = total;
    }

    public List<String> getOptions() {
        return options;
    }

    public void setOptions(List<String> options) {
        this.options = options;
    }

    public int getTimeout() {
        return timeout;
    }

    public void setTimeout(int timeout) {
        this.timeout = timeout;
    }
}
