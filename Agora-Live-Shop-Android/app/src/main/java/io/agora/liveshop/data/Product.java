package io.agora.liveshop.data;

import java.util.List;
import java.util.Map;

public class Product {
    private String mId;
    private String mTitle;
    private Map<String, List<String>> mVariants;
    private int mSold;
    private double mOriginalPrice;
    private double mGroupPrice;
    private String mCurrency;
    private int mStreamId;

    public String getId() {
        return mId;
    }

    public void setId(String mId) {
        this.mId = mId;
    }

    public String getTitle() {
        return mTitle;
    }

    public void setTitle(String mTitle) {
        this.mTitle = mTitle;
    }

    public Map<String, List<String>> getVariants() {
        return mVariants;
    }

    public void setVariants(Map<String, List<String>> mVariants) {
        this.mVariants = mVariants;
    }

    public int getSold() {
        return mSold;
    }

    public void setSold(int mSold) {
        this.mSold = mSold;
    }

    public double getOriginalPrice() {
        return mOriginalPrice;
    }

    public void setOriginalPrice(double mOriginalPrice) {
        this.mOriginalPrice = mOriginalPrice;
    }

    public double getGroupPrice() {
        return mGroupPrice;
    }

    public void setGroupPrice(double mGroupPrice) {
        this.mGroupPrice = mGroupPrice;
    }

    public String getCurrency() {
        return mCurrency;
    }

    public void setCurrency(String currency) {
        this.mCurrency = currency;
    }

    public int getStreamId() {
        return mStreamId;
    }

    public void setStreamId(int mStreamId) {
        this.mStreamId = mStreamId;
    }

    @Override
    public int hashCode() {
        return mId.hashCode() + mTitle.hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Product)) {
            return false;
        }

        Product p = (Product) obj;
        return mId.equals(p.mId) && mTitle.equals(p.mTitle) && mStreamId == p.mStreamId;
    }
}
