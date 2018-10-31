package io.agora.liveshop.data;

public class QuizResult {
    private int result;
    private int total;
    private int[] spread;
    private int optionCount;

    @Override
    public int hashCode() {
        return result + total + optionCount;
    }

    @Override
    public boolean equals(Object object) {
        if (!(object instanceof QuizResult)) {
            return false;
        }

        QuizResult result = (QuizResult) object;
        return this.result == result.result &&
                this.total == result.total &&
                this.optionCount == result.optionCount;
    }

    public int getResult() {
        return result;
    }

    public int getOptionCount() {
        return optionCount;
    }

    public void setOptionCount(int optionCount) {
        this.optionCount = optionCount;
    }

    public void setResult(int result) {
        this.result = result;
    }

    public int getTotal() {
        return total;
    }

    public void setTotal(int total) {
        this.total = total;
    }

    public int[] getSpread() {
        return spread;
    }

    public void setSpread(int[] spread) {
        this.spread = spread;
    }
}
