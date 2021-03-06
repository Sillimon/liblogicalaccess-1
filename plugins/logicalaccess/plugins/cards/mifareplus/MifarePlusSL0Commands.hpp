//
// Created by xaqq on 6/12/15.
//

#ifndef LIBLOGICALACCESS_MIFAREPLUSUTILS_H
#define LIBLOGICALACCESS_MIFAREPLUSUTILS_H

#include <logicalaccess/cards/commands.hpp>
#include <logicalaccess/plugins/cards/mifareplus/lla_cards_mifareplus_api.hpp>
#include <array>

namespace logicalaccess
{
#define CMD_MIFAREPLUSSL0 "MifarePlusSL0"

enum LLA_CARDS_MIFAREPLUS_API MifarePlusBlockKeyLocation : uint16_t
{
    MF_CARD_MASTER_KEY        = 0x9000,
    MF_CARD_CONFIGURATION_KEY = 0x9001,
    MF_L2_SWITCH_KEY          = 0x9002,
    MF_L3_SWITCH_KEY          = 0x9003,
    MF_L1_CARD_AUTH_KEY       = 0x9004
};

/**
 * This implements commands that can be use to personalize the card
 * when it is in security level 0.
 */
class LLA_CARDS_MIFAREPLUS_API MifarePlusSL0Commands : public Commands
{
  public:
    MifarePlusSL0Commands()
        : Commands(CMD_MIFAREPLUSSL0)
    {
    }

    explicit MifarePlusSL0Commands(std::string ct)
        : Commands(ct)
    {
    }

    virtual ~MifarePlusSL0Commands()
    {
    }
    int detectSecurityLevel() const;

    /**
     * Probe the card, attempting to detect if it *potentially* is a level 3 card.
     *
     * Send the "first authentication" command and check if it succeed.
     * If the card is SL3 it should succeed (even though we won't continue the
     * authentication)
     * otherwise it will fail.
     */
    bool probeLevel3() const;

    /**
     * Attempt to WRITE PERSO on the card to check if it is a level 0 card.
     *
     * @warning This is a "destructive operation" because if it succeed it will
     * rewrite a block on the card, potentially disrupting the user configuration.
     */
    bool isLevel0() const;

    /**
     * Issue the WRITE PERSO command, targeting location at
     * hex_addr_1 hex_addr_2
     */
    bool writePerso(uint8_t hex_addr_1, uint8_t hex_addr_2,
                    const std::array<uint8_t, 16> &data) const;

    /**
     * WRITE PERSO using an enum to chose the location.
     */
    bool writePerso(const MifarePlusBlockKeyLocation &location,
                    const std::array<uint8_t, 16> &data) const;

    /**
     * Send the COMMIT PERSO command, upgrading the card to level 1 or level 3
     * if it succeed.
     */
    bool commitPerso() const;

    /**
     * Attempt to deduce if the card is 4K or not.
     */
    bool is4K() const;

  protected:
    /**
     * Check whether or not this block is a sector trailer block.
     */
    static bool is_trailing_block(uint8_t hex_addr1, uint8_t hex_addr2);

    /**
     * Verify that the access bits are valid.
     * This is a partial verification and does not (yet ?) guarantee
     * that the access conditions are fully safe.
     */
    static bool validate_access_bytes(const std::array<uint8_t, 16> &data);
};
}

#endif // LIBLOGICALACCESS_MIFAREPLUSUTILS_H
